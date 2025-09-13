/**
 * simple_log.c
 *
 * @author : lostprobe
 * @date   : 2025/02/22 12:09
 * @brief  : simple_log.c
 */

#include "simple_log.h"
#include "internal/simple_log_sink.h"
#include "internal/simple_log_time.h"

static const char * log_level_to_string(log_level_t level);
static log_level_t log_level_ops(int ops, log_level_t *val);

enum {
    LOG_OP_GET = 0,
    LOG_OP_SET,
} LogLevelOps;

static const char * log_level_to_string(log_level_t level)
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

static log_level_t log_level_ops(int ops, log_level_t *val)
{
    static log_level_t level = LOG_LEVEL_DEFAULT;

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

log_level_t get_log_level()
{
    return log_level_ops(LOG_OP_GET, NULL);
}

int set_log_level(log_level_t level)
{
    if(level < LOG_LEVEL_OFF || level > LOG_LEVEL_DEBUG) // 添加越界判断
    {
        return SLOG_ERR_INVALID_ARG;
    }
    log_level_ops(LOG_OP_SET, &level);
    return SLOG_OK;
}

log_os_t simple_log_get_log_os()
{
    return slog_get_sink_fp();
}

int simple_log_set_log_os(log_os_t os)
{
    slog_set_sink(os, NULL, 0);
    return SLOG_OK;
}

int simple_log_set_log_file(const char *file)
{
    FILE* fp = NULL;
    int owned = 0;

    if(file == NULL)
        return SLOG_ERR_INVALID_ARG;
    if(0 == strcmp(file, "stdout"))
        fp = stdout;
    else if(0 == strcmp(file, "stderr"))
        fp = stderr;
    else
    {
        fp = fopen(file, "w");
        owned = 1;
    }
    if (fp == NULL)
        return SLOG_ERR_INVALID_ARG;
    slog_set_sink(fp, file, owned);
    return SLOG_OK;
}

char * simple_log_get_log_file()
{
    char * s = slog_get_sink_file();
    return s;
}

int simple_log_cleanup()
{
    return SLOG_OK;
}

int simple_log_level_vprintf(log_level_t level, const char * fmt, va_list args)
{
    int n = 0;
    log_os_t os = NULL;
    char *m_pos = NULL;
    int saved_errno = errno;
    char *fmt_new = fmt;
    char *err_s = NULL;
    int need_free = 0, new_len = 0;

    // 日志等级控制, 大于日志等级则不输出
    if(level > get_log_level())
        return SLOG_OK;

    if((os = simple_log_get_log_os()) == NULL)
    {
        // fprintf(stderr, "Invalid output stream");
        return SLOG_ERR_INVALID_SINK;
    }

    // 支持 %m 
    m_pos = strstr(fmt, "%m");
    // 检查 %%m 
    if(m_pos != NULL && *(m_pos - 1) != '%')
    { 
        // 将 fmt 中的 %m 替换为 strerror
        err_s = strerror(saved_errno);
        new_len = strlen(fmt) + strlen(err_s) + 1;
        need_free = 1;
        fmt_new = malloc(new_len * sizeof(char));
        memset(fmt_new, 0, new_len);
        strncpy(fmt_new, fmt, m_pos - fmt);
        strcat(fmt_new, err_s);
        strcat(fmt_new, m_pos + 2);
    }

    n = vfprintf(os, fmt_new, args);
    if(need_free)
    {
        free(fmt_new);
    }
    fflush(os);
    return n;
}

int simple_log_level_printf(log_level_t level, const char * fmt,...)
{
    int n = 0;
    va_list args;
    va_start(args, fmt);
    n = simple_log_level_vprintf(level, fmt, args);
    va_end(args);
    return n;
}

void simple_log_level_mesg(log_level_t level, const char * file, const char *func, int line, const char * fmt, ...)
{
    // print prefix: [LEVEL] [yyyy-mm-dd hh:mm:ss] func:line - 
    simple_log_level_printf(level, "[%5s] [%19s] [%s:%d] - ", log_level_to_string(level), slog_get_time(), func, line);
    va_list args;
    va_start(args, fmt);
    // print message
    simple_log_level_vprintf(level, fmt, args);
    va_end(args);
}

void simple_log_level_hexdump(int level, const char * file, const char *func, int line, const char *title, const void *begin, size_t s)
{
    uint8_t *p = (uint8_t *)begin;
    // print prefix: [LEVEL] [yyyy-mm-dd hh:mm:ss] func:line title:
    simple_log_level_printf(level, "[%5s] [%10s] [%s:%d] %s: ", log_level_to_string(level), slog_get_time(), func, line, title == NULL ? "" : title);
    for (int i = 0; i < s; i++)
    {
        if (i % 16 == 0)
            simple_log_level_printf(level, "\n\t");
        else if(i % 8 == 0)
            simple_log_level_printf(level, "  ");
        simple_log_level_printf(level, "%02x ", p[i]);
    }
    simple_log_level_printf(level, "\n");
}
