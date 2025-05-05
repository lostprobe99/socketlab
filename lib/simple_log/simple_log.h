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
    LOG_LEVEL_DEBUG = 1,
    LOG_LEVEL_INFO,
    LOG_LEVEL_WARN,
    LOG_LEVEL_ERROR,
    LOG_LEVEL_DIE,
    LOG_LEVEL_OFF = 255,
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

void log_level_mesg(LogLevel level, const char *file, const char *func, int line, const char * fmt, ...);
void log_level_hexdump(int level, const char * file, const char *func, int line, const char *title, const uint8_t *begin, size_t s);
void log_perror_impl(LogLevel level, const char * file, const char * func, int line, const char * fmt, ...);

#define LOG_DEBUG(fmt, ...) log_level_mesg(LOG_LEVEL_DEBUG, __FILE__, __FUNCTION__, __LINE__, fmt, ##__VA_ARGS__)
#define log_debug(fmt, ...) LOG_DEBUG(fmt, ##__VA_ARGS__)

#define LOG_INFO(fmt, ...) log_level_mesg(LOG_LEVEL_INFO, __FILE__, __FUNCTION__, __LINE__, fmt, ##__VA_ARGS__)
#define log_info(fmt, ...) LOG_INFO(fmt, ##__VA_ARGS__)

#define LOG_WARN(fmt, ...) log_level_mesg(LOG_LEVEL_WARN, __FILE__, __FUNCTION__, __LINE__, fmt, ##__VA_ARGS__)
#define log_warn(fmt, ...) LOG_WARN(fmt, ##__VA_ARGS__)

#define LOG_ERROR(fmt, ...) log_level_mesg(LOG_LEVEL_ERROR, __FILE__, __FUNCTION__, __LINE__, fmt, ##__VA_ARGS__)
#define log_error(fmt, ...) LOG_ERROR(fmt, ##__VA_ARGS__)

#define DIE(fmt, ...) do { \
    log_level_mesg(LOG_LEVEL_DIE, __FILE__, __FUNCTION__, __LINE__, fmt, ##__VA_ARGS__); \
    exit(1); \
} while(0)
#define log_die(fmt, ...) DIE(fmt, ##__VA_ARGS__)

#define LOG_HEXDUMP(level, title, begin, s) log_level_hexdump(level, __FILE__, __FUNCTION__, __LINE__, title, begin, s)
#define log_hexdump(level, title, begin, s) LOG_HEXDUMP(level, title, begin, s)
#define log_debug_hexdump(title, begin, s) LOG_HEXDUMP(LOG_LEVEL_DEBUG, title, begin, s)

#define log_perror(fmt, ...) log_perror_impl(LOG_LEVEL_ERROR, __FILE__, __FUNCTION__, __LINE__, fmt, ##__VA_ARGS__)

#ifdef __cplusplus
}
#endif

#endif // SIMPLE_LOG_H