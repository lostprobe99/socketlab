/**
 * debug.h
 *
 * @author : lostprobe
 * @date   : 2023/01/23 18:25
 * @brief  : debug.h
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

#define RED(format, ...) ANSI_FMT(format, ANSI_FG_RED), ##__VA_ARGS__
#define GREEN(format, ...) ANSI_FMT(format, ANSI_FG_GREEN), ##__VA_ARGS__
#define BLUE(format, ...) ANSI_FMT(format, ANSI_FG_BLUE), ##__VA_ARGS__

#define EXPECT(x1, x2)                                                                  \
    do                                                                                  \
    {                                                                                   \
        if ((x1) == (x2))                                                               \
            printf(GREEN("Test Passed: %s[%d] == %s[%d]\n", (#x1), (x1), (#x2), (x2))); \
        else                                                                            \
            printf(RED("Test Failed: %s[%d] != %s[%d]\n", (#x1), (x1), (#x2), (x2)));   \
    } while (0)

#define EVAL(expr, format) #expr ": " format "\n", expr 

#define Assert(cond, format, ...)                                      \
    do                                                                 \
    {                                                                  \
        if (!(cond))                                                   \
        {                                                              \
            printf(RED(format) "\n", ##__VA_ARGS__); \
            assert(cond);                                              \
        }                                                              \
    } while (0)

#define DIE(format, ...)                                                                                  \
    do                                                                                                           \
    {                                                                                                            \
        fprintf(stderr, RED("Fatal: %s:%d: " format) "", __FILE__, __LINE__, ##__VA_ARGS__); \
        exit(1);                                                                                                 \
    } while (0)

#define DIE_IF(cond, str)    \
    if (cond)               \
    {                       \
        perror(str);        \
        exit(EXIT_FAILURE); \
    }

#define ERROR(format, ...)                                                                                       \
    do                                                                                                           \
    {                                                                                                            \
        fprintf(stderr, RED("Error: %s:%d: " format) "", __FILE__, __LINE__, ##__VA_ARGS__); \
    } while (0)

#define WARN(format, ...)                                                                                          \
    do                                                                                                             \
    {                                                                                                              \
        fprintf(stderr, RED("Warning: %s:%d: " format) "", __FILE__, __LINE__, ##__VA_ARGS__); \
    } while (0)

#define INFO(format, ...)                                                                                         \
    do                                                                                                            \
    {                                                                                                             \
        fprintf(stderr, BLUE("Info: %s:%d: " format) "", __FILE__, __LINE__, ##__VA_ARGS__); \
    } while (0)
