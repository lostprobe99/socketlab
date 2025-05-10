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
} LogLevel;

#define LOG_LEVEL_DEFAULT LOG_LEVEL_INFO

#define LOG_OS_DEFAULT stdout

/// @brief FILE*
typedef FILE* log_os_t;

LogLevel get_log_level();
int set_log_level(LogLevel level);

log_os_t get_log_os();
int set_log_os(log_os_t os);
int set_log_os_file(const char *file);

int log_level_vprintf(LogLevel level, const char * fmt, va_list args);
int log_level_printf(LogLevel level, const char * fmt,...);

void log_level_mesg(LogLevel level, const char *file, const char *func, int line, const char * fmt, ...);
void log_level_hexdump(int level, const char * file, const char *func, int line, const char *title, const uint8_t *begin, size_t s);
void log_level_perror(LogLevel level, const char * file, const char * func, int line, const char * fmt, ...);

#define log_mesg(level, fmt, ...) log_level_mesg(level, __FILE__, __FUNCTION__, __LINE__, fmt, ##__VA_ARGS__)

#define log_debug(fmt, ...) log_mesg(LOG_LEVEL_DEBUG, fmt, ##__VA_ARGS__)
#define log_info(fmt, ...) log_mesg(LOG_LEVEL_INFO, fmt, ##__VA_ARGS__)
#define log_warn(fmt, ...) log_mesg(LOG_LEVEL_WARN, fmt, ##__VA_ARGS__)
#define log_error(fmt, ...) log_mesg(LOG_LEVEL_ERROR, fmt, ##__VA_ARGS__)
#define log_fatal(fmt, ...) log_mesg(LOG_LEVEL_FATAL, fmt, ##__VA_ARGS__)

#define die(fmt, ...) do { \
    log_mesg(LOG_LEVEL_FATAL, fmt, ##__VA_ARGS__); \
    exit(1); \
} while(0)

#define log_die(fmt, ...) die(fmt, ##__VA_ARGS__)

#define log_hexdump(level, title, begin, s) log_level_hexdump(level, __FILE__, __FUNCTION__, __LINE__, title, begin, s)
#define log_debug_hexdump(title, begin, s) log_hexdump(LOG_LEVEL_DEBUG, title, begin, s)

#define log_perror(level, fmt, ...) log_level_perror(level, __FILE__, __FUNCTION__, __LINE__, fmt, ##__VA_ARGS__)
#define log_eperror(fmt, ...) log_perror(LOG_LEVEL_ERROR, fmt, ##__VA_ARGS__)
#define log_fperror(fmt, ...) log_perror(LOG_LEVEL_FATAL, fmt, ##__VA_ARGS__)

#ifdef __cplusplus
}
#endif

#endif // SIMPLE_LOG_H