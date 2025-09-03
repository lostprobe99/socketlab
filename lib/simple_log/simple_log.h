/**
 * simple_log.h
 *
 * @author : lostprobe
 * @date   : 2025/02/22 12:09
 * @brief  : simple_log.h
 */

#ifndef SIMPLE_LOG_H
#define SIMPLE_LOG_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <stdint.h>
#include <errno.h>
#include <time.h>
#include <unistd.h>
#include <sys/time.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    LOG_LEVEL_OFF = 0,
    LOG_LEVEL_FATAL,
    LOG_LEVEL_ERROR,
    LOG_LEVEL_WARN,
    LOG_LEVEL_INFO,
    LOG_LEVEL_DEBUG,
} log_level_t;

typedef enum {
    SLOG_OK = 0,
    SLOG_ERR_INVALID_ARG = -1,
    SLOG_ERR_INVALID_SINK = -2,
} slog_error_code_t;

#define LOG_LEVEL_DEFAULT LOG_LEVEL_INFO

#define LOG_OS_DEFAULT stdout

/// @brief FILE*
typedef FILE* log_os_t;

log_level_t get_log_level();
int set_log_level(log_level_t level);

log_os_t simple_log_get_log_os();
int simple_log_set_log_os(log_os_t os);

int simple_log_set_log_file(const char *file);
char * simple_log_get_log_file();

int simple_log_level_vprintf(log_level_t level, const char * fmt, va_list args);
int simple_log_level_printf(log_level_t level, const char * fmt,...);

void simple_log_level_mesg(log_level_t level, const char *file, const char *func, int line, const char * fmt, ...);
void simple_log_level_hexdump(int level, const char * file, const char *func, int line, const char *title, const uint8_t *begin, size_t s);

#define slog_mesg(level, fmt, ...) simple_log_level_mesg(level, __FILE__, __FUNCTION__, __LINE__, fmt, ##__VA_ARGS__)

#define log_debug(fmt, ...) slog_mesg(LOG_LEVEL_DEBUG, fmt, ##__VA_ARGS__)
#define log_info(fmt, ...) slog_mesg(LOG_LEVEL_INFO, fmt, ##__VA_ARGS__)
#define log_warn(fmt, ...) slog_mesg(LOG_LEVEL_WARN, fmt, ##__VA_ARGS__)
#define log_error(fmt, ...) slog_mesg(LOG_LEVEL_ERROR, fmt, ##__VA_ARGS__)
#define log_fatal(fmt, ...) slog_mesg(LOG_LEVEL_FATAL, fmt, ##__VA_ARGS__)

#define die(fmt, ...) do { \
    slog_mesg(LOG_LEVEL_FATAL, fmt, ##__VA_ARGS__); \
    exit(1); \
} while(0)

#define log_die(fmt, ...) die(fmt, ##__VA_ARGS__)

#define log_hexdump(level, title, begin, s) simple_log_level_hexdump(level, __FILE__, __FUNCTION__, __LINE__, title, begin, s)
#define log_debug_hexdump(title, begin, s) log_hexdump(LOG_LEVEL_DEBUG, title, begin, s)

#ifdef __cplusplus
}
#endif

#endif // SIMPLE_LOG_H