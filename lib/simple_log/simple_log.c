/**
 * simple_log.c
 *
 * @author : lostprobe
 * @date   : 2025/02/22 12:09
 * @brief  : simple_log.c
 */

#include "simple_log.h"

static const char * log_level_to_string(LogLevel level);
static LogLevel log_level_ops(int ops, LogLevel *val);
static log_os_t log_output_stream_ops(int ops, log_os_t* val);
static const char * log_get_time();

static const char * log_level_to_string(LogLevel level)
{
    const static char * log_level_str[] = {
        "",
        "FATAL",
        "ERROR",
        "WARN",
        "INFO",
        "DEBUG",
    };
    if(level < LOG_LEVEL_OFF || level > LOG_LEVEL_DEBUG) // 更新越界判断
    {
        return "INVALID_LEVEL";
    }
    return log_level_str[level];
}

enum {
    LOG_OP_GET = 0,
    LOG_OP_SET,
} LogLevelOps;

static LogLevel log_level_ops(int ops, LogLevel *val)
{
    static LogLevel level = LOG_LEVEL_DEFAULT;

    if(val == NULL)
        return level;

    switch(ops)
    {
        case LOG_OP_GET:
            *val = level;
            break;
        case LOG_OP_SET:
            level = *val;
            break;
        default:
            break;
    }
    return level;
}

LogLevel get_log_level()
{
    return log_level_ops(LOG_OP_GET, NULL);
}

int set_log_level(LogLevel level)
{
    log_level_ops(LOG_OP_SET, &level);
    return 0;
}

static log_os_t log_output_stream_ops(int ops, log_os_t* val)
{
    static log_os_t os = NULL;

    if(os == NULL)
        os = LOG_OS_DEFAULT;

    if(val == NULL)
        return os;

    switch(ops)
    {
        case LOG_OP_GET:
            *val = os;
            break;
        case LOG_OP_SET:
            os = *val;
            break;
        default:
            break;
    }
    return os;
}

log_os_t get_log_os()
{
    return log_output_stream_ops(LOG_OP_GET, NULL);
}

int set_log_os(log_os_t os)
{
    log_output_stream_ops(LOG_OP_SET, &os);
    return 0;
}

int set_log_os_file(const char *file)
{
    FILE* fp = fopen(file, "w");
    if (fp == NULL)
        return -1;
    set_log_os(fp);
    return 0;
}

static const char * log_get_time()
{
    time_t t;
    struct tm *tmp;
    static char buf[64];
    struct timeval tv;
    int ms;

    t = time(NULL);
    tmp = localtime(&t);
    gettimeofday(&tv, NULL);
    ms = tv.tv_usec / 1000;  // Convert microseconds to milliseconds

    strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", tmp);
    snprintf(buf + strlen(buf), sizeof(buf) - strlen(buf), ".%03d", ms);  // Append milliseconds

    return buf;
}

int log_level_vprintf(LogLevel level, const char * fmt, va_list args)
{
    int n = 0;
    log_os_t os = NULL;

    if(level > get_log_level())
        return -1;

    if((os = get_log_os()) == NULL)
    {
        fprintf(stderr, "Invalid output stream");
        return -1;
    }
    n = vfprintf(os, fmt, args);
    fflush(os);
    return n;
}

int log_level_printf(LogLevel level, const char * fmt,...)
{
    int n = 0;
    va_list args;
    va_start(args, fmt);
    n = log_level_vprintf(level, fmt, args);
    va_end(args);
    return n;
}

void log_level_mesg(LogLevel level, const char * file, const char *func, int line, const char * fmt, ...)
{
    // print prefix: [LEVEL] [yyyy-mm-dd hh:mm:ss] func:line - 
    log_level_printf(level, "[%5s] [%19s] [%s:%d] - ", log_level_to_string(level), log_get_time(), func, line);
    va_list args;
    va_start(args, fmt);
    // print message
    log_level_vprintf(level, fmt, args);
    va_end(args);
}

void log_level_perror(LogLevel level, const char * file, const char * func, int line, const char * fmt, ...)
{
    int errno_save = errno;
    va_list args;
    va_start(args, fmt);
    // print prefix: [LEVEL] [yyyy-mm-dd hh:mm:ss] func:line - 
    log_level_printf(level, "[%5s] [%19s] [%s:%d] - ", log_level_to_string(level), log_get_time(), func, line);
    // print message
    log_level_vprintf(level, fmt, args);
    va_end(args);
    // print errorno meesage
    log_level_printf(level, ": %s\n", strerror(errno_save));
}

void log_level_hexdump(int level, const char * file, const char *func, int line, const char *title, const uint8_t *begin, size_t s)
{
    // print prefix: [LEVEL] [yyyy-mm-dd hh:mm:ss] func:line title:
    log_level_printf(level, "[%5s] [%10s] [%s:%d] %s: ", log_level_to_string(level), log_get_time(), func, line, title == NULL ? "" : title);
    for (int i = 0; i < s; i++)
    {
        if (i % 16 == 0)
            log_level_printf(level, "\n\t");
        else if(i % 8 == 0)
            log_level_printf(level, "  ");
        log_level_printf(level, "%02x ", begin[i]);
    }
    log_level_printf(level, "\n");
}
