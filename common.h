/**
 * common.h
 *
 * @Author : lostprobe
 * @Date   : 2022/02/13 18:56
 * @Brief  : common.h
 */

#ifndef COMMON_H
#define COMMON_H

#ifdef WIN32

#include <winsock2.h>
#include <io.h>
#define sleep _sleep

#elif linux

#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <sys/stat.h>

#define INVALID_SOCKET -1
#define SOCKET_ERROR -1
typedef struct sockaddr_in SOCKADDR_IN;
typedef struct sockaddr SOCKADDR;
typedef int SOCKET;
#endif

#define ERROR_HANDLING(str)                         \
    do                                              \
    {                                               \
        char buf[256], *fmt = "%s:%d: %s";        \
        sprintf(buf, fmt, __FILE__, __LINE__, str); \
        perror(buf);                                \
        exit(1);                                    \
    } while (0)

#define ERROR_P(str)                         \
    do                                              \
    {                                               \
        char buf[256], *fmt = "%s:%d: %s";        \
        sprintf(buf, fmt, __FILE__, __LINE__, str); \
        perror(buf);                                \
    } while (0)

#endif // COMMON_H