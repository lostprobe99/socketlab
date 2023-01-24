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
    struct hostent* h = gethostbyname(addr);
    if(!h)
        FATAL_EXIT("addr `%s` error", addr);
    sockaddr_in sock_addr;
    memset(&sock_addr, 0, sizeof(sockaddr_in));
    sock_addr.sin_family = af;
    // sock_addr.sin_addr.s_addr = (inet_addr(h->h_addr_list[0]));  // 写错力，做个纪念把
    sock_addr.sin_addr = *((struct in_addr*)(h->h_addr_list[0]));
    sock_addr.sin_port = htons(port);
    return sock_addr;
}