#include "InetAddr.h"
#include "debug.h"
#include <netdb.h>
#include <cstring>

std::string to_string(const InetAddr& value)
{
    const char * fmt = "InetAddr{ addr = \"%s\", port = %u }";
    char buf[64]{};
    snprintf(buf, 64, fmt, value.addr().c_str(), value.port());
    return (buf);
}

std::ostream& operator<<(std::ostream& os, const InetAddr& rhs)
{
    os << to_string(rhs);
    return os;
}

InetAddr::InetAddr() : _addr_len(sizeof(_addr))
{
    bzero(&_addr, sizeof(_addr));
}

InetAddr::InetAddr(const char * addr, uint16_t port)
{
    bzero(&_addr, sizeof(_addr));
    _addr.sin_family = AF_INET;
    _addr.sin_addr.s_addr = (INADDR_ANY);
    _addr.sin_port = htons(port);
    _addr_len = sizeof(_addr);
    if(addr != NULL)
    {
        struct hostent* h = gethostbyname(addr);
        if(h == NULL)
        {
            FATAL("addr = `%s` error", addr);
            exit(1);
        }
        this->_addr.sin_addr = *((struct in_addr*)(h->h_addr_list[0]));
    }
}

std::string InetAddr::addr() const
{
    std::string ip = inet_ntoa(this->_addr.sin_addr);
    return ip;
}

void InetAddr::addr(std::string addr)
{
    this->_addr.sin_addr.s_addr = inet_addr(INADDR_ANY);
    if(!addr.empty())
    {

        struct hostent* h = gethostbyname(addr.c_str());
        if(h == NULL)
        {
            FATAL("addr = `%s` error", addr.c_str());
            exit(1);
        }
        this->_addr.sin_addr = *((struct in_addr*)(h->h_addr_list[0]));
    }
        // this->_addr.sin_addr.s_addr = inet_addr(addr.c_str());
}

uint16_t InetAddr::port() const
{
    return ntohs(this->_addr.sin_port);
}

void InetAddr::port(uint16_t port)
{
    this->_addr.sin_port = htons(port);
}

uint16_t InetAddr::af() const
{
    return this->_addr.sin_family;
}

InetAddr::~InetAddr()
{

}
