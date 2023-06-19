#pragma once

#include <arpa/inet.h>
#include <string>

class InetAddr
{
private:
    friend std::string to_string(const InetAddr& );
    friend std::ostream& operator<<(std::ostream& os, const InetAddr&);
    friend class Socket;

    struct sockaddr_in _addr;
    socklen_t _addr_len;

public:
    InetAddr();
    InetAddr(const char * addr, uint16_t port);
    std::string addr() const;
    void addr(std::string addr);
    uint16_t port() const;
    void port(uint16_t port);
    uint16_t af() const;
    ~InetAddr();
};