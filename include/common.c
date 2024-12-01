#include <string.h>
#include <netdb.h>

#include <sys/time.h> // for struct timeval

#include "common.h"
#include "debug.h"

#if defined(IS_GNU_C) || defined(IS_GNU_CXX)
// void *p 表示接收任意类型的参数
void auto_free(void *p)
{
    // 实际上得到的参数是变量的地址
    void **q = (void**)p;
    free(*q);
}
#endif

socket_t make_socket(int af, int type, int protocol)
{
    #ifdef IS_WINDOWS
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
        return INVALID_SOCKET;
    #endif

    return socket(af, type, protocol);
}

sockaddr_in make_sockaddr(int af, const char* addr, unsigned short port)
{
    sockaddr_in sock_addr;
    memset(&sock_addr, 0, sizeof(sockaddr_in));
    sock_addr.sin_family = af;
    sock_addr.sin_port = htons(port);
    sock_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    if(addr != NULL)
    {
        struct hostent* h = gethostbyname(addr);
        if(!h)
            DIE("addr `%s` error", addr);
        // sock_addr.sin_addr.s_addr = (inet_addr(h->h_addr_list[0]));  // 写错力，做个纪念把
        sock_addr.sin_addr = *((struct in_addr*)(h->h_addr_list[0]));
    }
    return sock_addr;
}

socket_t server(unsigned short port)
{
    socket_t fd = make_socket(AF_INET, SOCK_STREAM, 0);
    if(fd == INVALID_SOCKET)
        return -1;
    
    // 设置 SO_REUSEADDR
    int sock_opt = 1, optlen = sizeof(sock_opt);
    setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, (void*)&sock_opt, optlen);

    sockaddr_in servAddr = make_sockaddr(AF_INET, NULL, port);

    if(bind(fd, (sockaddr *)&servAddr, sizeof(servAddr)) == SOCKET_ERROR)
        return -1;

    if(listen(fd, 5) == SOCKET_ERROR)
        return -1;

    return fd;
}

socket_t c_connect(const char * addr, unsigned short port)
{
    int fd = make_socket(AF_INET, SOCK_STREAM, 0);
    if(fd == INVALID_SOCKET)
        return -1;
    sockaddr_in serv_addr = make_sockaddr(AF_INET, addr, port);
    
    if(connect(fd, (sockaddr*)&serv_addr, sizeof(serv_addr)) == SOCKET_ERROR)
        return -1;

    return fd;
}

int tcp_connected(socket_t fd)
{
    struct tcp_info info;
    int info_size = sizeof(info);
    if(getsockopt(fd, IPPROTO_TCP, TCP_INFO, &info, (socklen_t*)&info_size) != 0)
        return 0;
    return info.tcpi_state == TCP_ESTABLISHED;
}

int get_tcp_info(socket_t fd, struct tcp_info* info)
{
    int info_size = sizeof(struct tcp_info);
    return getsockopt(fd, IPPROTO_TCP, TCP_INFO, info, (socket_t*)&info_size);
}

int set_broadcast(socket_t fd)
{
    // 设置广播
    int val = 1;
    return setsockopt(fd, SOL_SOCKET, SO_BROADCAST, &val, sizeof(val));
}

int set_recv_timeout(socket_t fd, unsigned int sec, unsigned int usec)
{
    struct timeval tv = {
        tv.tv_sec = sec,
        tv.tv_usec = 0,
    };
    return setsockopt(fd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));
}

int set_send_timeout(socket_t fd, unsigned int sec, unsigned int usec)
{
    struct timeval tv = {
        tv.tv_sec = sec,
        tv.tv_usec = 0,
    };
    return setsockopt(fd, SOL_SOCKET, SO_SNDTIMEO, &tv, sizeof(tv));
}