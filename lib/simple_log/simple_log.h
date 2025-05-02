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
    LOG_OFF = 0,
    LOG_DEBUG = 1,
    LOG_INFO,
    LOG_WARN,
    LOG_ERROR,
    LOG_DIE,
} LogLevel;

#define LOG_OS_STDOUT stdout

/// @brief FILE*
typedef FILE* log_os_t;

LogLevel get_log_level();
int set_log_level(LogLevel level);

log_os_t get_log_os();
int set_log_os(log_os_t os);
int set_log_os_file(const char *file);

// const char * log_get_time();

void log_mesg(LogLevel level, const char *file, const char *func, int line, const char * fmt, ...);
void log_hexdump(int level, const char * file, const char *func, int line, const char *title, const uint8_t *begin, size_t s);

#define LOG_DEBUG(fmt, ...) log_mesg(LOG_DEBUG, __FILE__, __FUNCTION__, __LINE__, fmt, ##__VA_ARGS__)
#define log_debug LOG_DEBUG

#define LOG_INFO(fmt, ...) log_mesg(LOG_INFO, __FILE__, __FUNCTION__, __LINE__, fmt, ##__VA_ARGS__)
#define log_info LOG_INFO

#define LOG_WARN(fmt, ...) log_mesg(LOG_WARN, __FILE__, __FUNCTION__, __LINE__, fmt, ##__VA_ARGS__)
#define log_warn LOG_WARN

#define LOG_ERROR(fmt, ...) log_mesg(LOG_ERROR, __FILE__, __FUNCTION__, __LINE__, fmt, ##__VA_ARGS__)
#define log_error LOG_ERROR

#define DIE(fmt, ...) do { \
    log_mesg(LOG_DIE, __FILE__, __FUNCTION__, __LINE__, fmt, ##__VA_ARGS__); \
    exit(1); \
} while(0)
#define log_die DIE

#define LOG_LEVEL_HEXDUMP(level, title, begin, s) log_hexdump(level, __FILE__, __FUNCTION__, __LINE__, title, begin, s)
#define log_debug_hexdump(title, begin, s) LOG_LEVEL_HEXDUMP(LOG_DEBUG, title, begin, s)

#ifdef __cplusplus
}
#endif

#endif // SIMPLE_LOG_H