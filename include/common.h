/**
 * common.h
 *
 * @Author : lostprobe
 * @Date   : 2022/02/13 18:56
 * @Brief  : common.h
 */

#ifndef COMMON_H
#define COMMON_H

#if defined(_WIN32) || defined(_WIN64)

#include <winsock2.h>
#include <io.h>

#define sleep _sleep

typedef int socklen_t;
typedef SOCKET socket_t;
typedef SOCKADDR_IN sockaddr_in;
typedef SOCKADDR sockaddr;

#elif linux

#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/wait.h>

#define INVALID_SOCKET -1
#define SOCKET_ERROR -1
typedef struct sockaddr_in sockaddr_in;
typedef struct sockaddr sockaddr;
typedef int SOCKET;
typedef int socket_t;
#endif

socket_t make_socket(int af, int type, int protocol);
sockaddr_in make_sockaddr(int af, const char* addr, unsigned short port);

#define ERROR_HANDLING(str)                         \
    do                                              \
    {                                               \
        char buf[256], *fmt = "%s:%d: %s";          \
        sprintf(buf, fmt, __FILE__, __LINE__, str); \
        perror(buf);                                \
        exit(1);                                    \
    } while (0)

#define ERROR_P(str)                                \
    do                                              \
    {                                               \
        char buf[256], *fmt = "%s:%d: %s";          \
        sprintf(buf, fmt, __FILE__, __LINE__, str); \
        perror(buf);                                \
    } while (0)

#endif // COMMON_H