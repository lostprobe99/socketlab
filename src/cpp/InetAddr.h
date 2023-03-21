#pragma once

#include <arpa/inet.h>

class InetAddr
{
public:
    struct sockaddr_in _addr;
    socklen_t _addr_len;

    InetAddr();
    InetAddr(const char * addr, uint16_t port);
    ~InetAddr();
};