/**
 * common.h
 *
 * @author : lostprobe
 * @date   : 2022/02/13 18:56
 * @brief  : common.h
 */

#ifndef COMMON_H
#define COMMON_H

#if defined(__GNUC__)
extern void auto_free(void *p);
    #define AUTOFREE __attribute__((__cleanup__(auto_free)))
#else
    #define AUTOFREE assert(0)
#endif

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
// #include <netinet/tcp.h>
#include <linux/tcp.h>

#define INVALID_SOCKET -1
#define SOCKET_ERROR -1
typedef struct sockaddr_in sockaddr_in;
typedef struct sockaddr sockaddr;
typedef int socket_t;
#endif


socket_t make_socket(int af, int type, int protocol);

/// @brief 创建 sockaddr_in 结构体
/// @param af 对应 sockaddr_in.sin_family
/// @param addr 对应 sockaddr_in.sin_addr
/// @param port 对应 sockaddr_in.sin_port
/// @note 如果 addr 为 NULL，则使用 INADDR_ANY
sockaddr_in make_sockaddr(int af, const char* addr, unsigned short port);

/// @brief 在本机创建一个 TCP 服务端
/// @param port 要使用的端口
/// @return 服务端 socket
/// @retval -1 - 如果出错
/// @note 该函数会设置 SO_REUSEADDR
socket_t server(unsigned short port);

/// @brief 连接到一个 TCP 服务器
/// @param addr 服务器地址
/// @param port 端口
/// @return 服务器 socket
/// @retval -1 - 如果出错
socket_t c_connect(const char * addr, unsigned short port);

/// @brief 检测所给 socket 是否已连接
/// @param fd 要检测的 socket
/// @retval 1 - fd 已连接
/// @retval 0 - 失败或未连接
int tcp_connected(socket_t fd);

/// @brief 获取 tcp info
/// @param fd socket
/// @param info 用于保存 tcp info 的 struct tcp_info 指针
/// @retval 0 - 获取成功
/// @retval 1 - 失败
int get_tcp_info(socket_t fd, struct tcp_info* info);

#endif // COMMON_H