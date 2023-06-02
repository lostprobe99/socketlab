#include "Socket.h"
#include "InetAddr.h"
#include "debug.h"

#include <unistd.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <errno.h>
#include <cstring>

Socket::Socket() : _fd(-1)
{
    _fd = socket(AF_INET, SOCK_STREAM, 0);
    errif(_fd == -1, "socket create error");
}

Socket::Socket(int fd) : _fd(fd)
{
    errif(_fd == -1, "socket create error");
}

Socket::~Socket()
{
    if(_fd != -1)
    {
        close(_fd);
        _fd = -1;
    }
}

void Socket::bind(InetAddr * addr)
{
    errif(::bind(_fd, (struct sockaddr*)&addr->_addr, addr->_addr_len) == -1, "bind error");
}

void Socket::listen()
{
    errif(::listen(_fd, 5) == -1, "listen error");
}

void Socket::setnonblocking()
{
    fcntl(_fd, F_SETFL, fcntl(_fd, F_GETFL) | O_NONBLOCK);
}

int  Socket::accept(InetAddr * clnt_addr)
{
    int clnt_fd = ::accept(_fd, (struct sockaddr*)&clnt_addr->_addr, &clnt_addr->_addr_len);
    errif(clnt_fd == -1, "accept error");
    return clnt_fd;
}

int Socket::get_fd()
{
    return _fd;
}
