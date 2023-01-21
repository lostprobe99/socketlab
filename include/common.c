#include <string.h>

#include "common.h"

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
    sock_addr.sin_addr.s_addr = (inet_addr(addr));
    sock_addr.sin_port = htons(port);
    return sock_addr;
}