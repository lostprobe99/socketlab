#include "InetAddr.h"
#include <cstring>


InetAddr::InetAddr() : _addr_len(sizeof(_addr))
{
    bzero(&_addr, sizeof(_addr));
}

InetAddr::InetAddr(const char * addr, uint16_t port)
{
    bzero(&_addr, sizeof(_addr));
    _addr.sin_family = AF_INET;
    _addr.sin_addr.s_addr = inet_addr(addr);
    _addr.sin_port = htons(port);
    _addr_len = sizeof(_addr);
}

InetAddr::~InetAddr()
{

}
