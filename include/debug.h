/**
 * debug.h
 *
 * @Author : lostprobe
 * @Date   : 2023/01/23 18:25
 * @Brief  : debug.h
 */

#pragma once

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#define ANSI_FG_BLACK   "\33[1;30m"
#define ANSI_FG_RED     "\33[1;31m"
#define ANSI_FG_GREEN   "\33[1;32m"
#define ANSI_FG_YELLOW  "\33[1;33m"
#define ANSI_FG_BLUE    "\33[1;34m"
#define ANSI_FG_MAGENTA "\33[1;35m"
#define ANSI_FG_CYAN    "\33[1;36m"
#define ANSI_FG_WHITE   "\33[1;37m"

#define ANSI_BG_BLACK   "\33[1;40m"
#define ANSI_BG_RED     "\33[1;41m"
#define ANSI_BG_GREEN   "\33[1;42m"
#define ANSI_BG_YELLOW  "\33[1;43m"
#define ANSI_BG_BLUE    "\33[1;44m"
#define ANSI_BG_MAGENTA "\33[1;45m"
#define ANSI_BG_CYAN    "\33[1;46m"
#define ANSI_BG_WHITE   "\33[1;47m"

#define ANSI_NONE       "\33[0m"

#define ANSI_FMT(str, color) color str ANSI_NONE

#define Assert(cond, format, ...)                                      \
    do                                                                 \
    {                                                                  \
        if (!(cond))                                                   \
        {                                                              \
            printf(ANSI_FMT(format, ANSI_FG_RED) "\n", ## __VA_ARGS__);      \
            assert(cond);                                              \
        }                                                              \
    } while (0)

#define FATAL_EXIT(format, ...)                                                                              \
    do                                                                                                        \
    {                                                                                                         \
        fprintf(stderr, ANSI_FMT("Fatal: %s:%d: " format, ANSI_BG_RED) "\n", __FILE__, __LINE__, ##__VA_ARGS__); \
        exit(1);                                                                                          \
    } while (0)

#define FATAL_RET(ret, format, ...)                                                                              \
    do                                                                                                        \
    {                                                                                                         \
        fprintf(stderr, ANSI_FMT("Fatal: %s:%d: " format, ANSI_BG_RED) "\n", __FILE__, __LINE__, ##__VA_ARGS__); \
        return ret;\
    } while (0)

#define FATAL(format, ...)                                                                              \
    do                                                                                                        \
    {                                                                                                         \
        fprintf(stderr, ANSI_FMT("Fatal: %s:%d: " format, ANSI_BG_RED) "\n", __FILE__, __LINE__, ##__VA_ARGS__); \
    } while (0)

#define WARN(format, ...)                                                                              \
    do                                                                                                        \
    {                                                                                                         \
        fprintf(stderr, ANSI_FMT("Warning: %s:%d: " format, ANSI_FG_RED) "\n", __FILE__, __LINE__, ##__VA_ARGS__); \
    } while (0)