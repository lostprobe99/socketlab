#include <string.h>
#include <netdb.h>

#include "common.h"
#include "debug.h"

socket_t make_socket(int af, int type, int protocol)
{
    #if defined(_WIN32) || defined(_WIN64)
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
            FATAL_EXIT("addr `%s` error", addr);
        // sock_addr.sin_addr.s_addr = (inet_addr(h->h_addr_list[0]));  // 写错力，做个纪念把
        sock_addr.sin_addr = *((struct in_addr*)(h->h_addr_list[0]));
    }
    return sock_addr;
}

socket_t server(unsigned short port)
{
    socket_t fd = make_socket(AF_INET, SOCK_STREAM, 0);
    Assert(fd != INVALID_SOCKET, "socket() error");
    
    // 设置 SO_REUSEADDR
    int sock_opt = 1, optlen = sizeof(sock_opt);
    setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, (void*)&sock_opt, optlen);

    sockaddr_in servAddr = make_sockaddr(AF_INET, NULL, port);

    Assert(bind(fd, (sockaddr *)&servAddr, sizeof(servAddr)) != SOCKET_ERROR, "bind() error");

    Assert(listen(fd, 5) != SOCKET_ERROR, "listen() error");

    return fd;
}