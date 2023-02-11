/**
 * common.h
 *
 * @author : lostprobe
 * @date   : 2022/02/13 18:56
 * @brief  : common.h
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

#elif __linux

#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/wait.h>

#define INVALID_SOCKET -1
#define SOCKET_ERROR -1
typedef struct sockaddr_in sockaddr_in;
typedef struct sockaddr sockaddr;
typedef int socket_t;
#endif

socket_t make_socket(int af, int type, int protocol);
sockaddr_in make_sockaddr(int af, const char* addr, unsigned short port);
/// @brief 在本机创建一个 TCP 服务端
/// @param port 要使用的端口
/// @return 服务端 socket
socket_t server(unsigned short port);

#endif // COMMON_H