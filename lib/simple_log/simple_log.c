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

static const char * log_level_to_string(LogLevel level)
{
    const static char * log_level_str[] = {
        "OFF",
        "DEBUG",
        "INFO",
        "WARN",
        "ERROR",
        "DIE",
    };
    if(level > LOG_DIE || level < LOG_OFF)
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
    static LogLevel level = LOG_DEBUG;

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
        os = stdout;

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

const char * log_get_time()
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

void log_mesg(LogLevel level, const char * file, const char *func, int line, const char * fmt, ...)
{
    log_os_t os = get_log_os();
    if(os == NULL)
    {
        fprintf(stderr, "Invalid output stream");
        return;
    }
    if(level < get_log_level())
        return;
    int errno_save = errno;
    va_list args;
    va_start(args, fmt);
    // print prefix: [LEVEL] [yyyy-mm-dd hh:mm:ss] func:line - 
    fprintf(os, "[%5s] [%19s] [%s:%d] - ", log_level_to_string(level), log_get_time(), func, line);
    // print message
    vfprintf(os, fmt, args);
    va_end(args);
    fflush(os);
}

void log_perror(LogLevel level, const char * file, const char * func, int line, const char * fmt, ...)
{
    
}

void log_hexdump(int level, const char * file, const char *func, int line, const char *title, const uint8_t *begin, size_t s)
{
    log_os_t os = get_log_os();
    if(os == NULL)
    {
        fprintf(stderr, "Invalid output stream");
        return;
    }
    if(level < get_log_level())
        return;
    int errno_save = errno;
    // print prefix: [LEVEL] [yyyy-mm-dd hh:mm:ss] func:line title:
    fprintf(os, "[%5s] [%10s] [%s:%d] %s: ", log_level_to_string(level), log_get_time(), func, line, title == NULL ? "" : title);
    for (int i = 0; i < s; i++)
    {
        if (i % 16 == 0)
            fprintf(os, "\n\t");
        fprintf(os, "%02x ", begin[i]);
    }
    fprintf(os, "\n");
    fflush(os);
}
