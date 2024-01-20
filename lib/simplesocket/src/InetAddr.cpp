#include "InetAddr.h"
#include "debug.h"
#include <netdb.h>
#include <cstring>
#include <stdexcept>

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
    _addr_len = sizeof(_addr);
    this->addr(addr);
    this->port(port);
}

std::string InetAddr::addr() const
{
    std::string ip = inet_ntoa(this->_addr.sin_addr);
    return ip;
}

void InetAddr::addr(std::string addr)
{
    this->_addr.sin_addr.s_addr = (INADDR_ANY);
    if(!addr.empty())
    {
        struct hostent* h = gethostbyname(addr.c_str());
        if(h == NULL)
        {
            throw std::runtime_error("Failed to resolve hostname: gethostbyname() returned NULL, herrno: " + std::string(hstrerror(errno)));
        }
        this->_addr.sin_addr = *((struct in_addr*)(h->h_addr_list[0]));
    }
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

void InetAddr::af(uint16_t family)
{
    _addr.sin_family = family;
}

InetAddr::~InetAddr()
{

}
