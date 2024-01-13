#include "Socket.h"
#include "InetAddr.h"
#include "debug.h"

#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <errno.h>
#include <cstring>

std::string to_string(const Socket& value)
{
    const char * fmt = "Socket{ fd = %d }";
    char buf[32]{};
    snprintf(buf, 32, fmt, value._fd);
    return buf;
}

std::ostream& operator<<(std::ostream& os, const Socket& rhs)
{
    os << to_string(rhs);
    return os;
}

Socket::Socket() : _fd(-1)
{
}

Socket::Socket(int fd) : _fd(fd)
{
    errif(_fd == -1, "socket create error");
}

Socket::Socket(Socket &&other) noexcept : _fd(other._fd)
{
    other._fd = -1;
}

Socket& Socket::operator=(Socket&& other)
{
    if (this != &other)
    {
        this->close();
        this->_fd = other._fd;
        other._fd = -1;
    }
    return *this;
}

Socket Socket::open()
{
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    // 设置 SO_REUSEADDR
    int sock_opt = 1, optlen = sizeof(sock_opt);
    setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, (void*)&sock_opt, optlen);
    return Socket(fd);
}

Socket::~Socket()
{
    close();
}

void Socket::bind(const InetAddr& addr)
{
    errif(::bind(_fd, (struct sockaddr*)&addr._addr, addr._addr_len) == -1, "bind error");
}

void Socket::listen()
{
    errif(::listen(_fd, 5) == -1, "listen error");
}

void Socket::setnonblocking()
{
    fcntl(_fd, F_SETFL, fcntl(_fd, F_GETFL) | O_NONBLOCK);
}

// Socket Socket::accept(InetAddr& clnt_addr)
int Socket::accept(InetAddr& clnt_addr)
{
    int clnt_fd = ::accept(_fd, (struct sockaddr*)&clnt_addr._addr, &clnt_addr._addr_len);
    errif(clnt_fd == -1, "accept error");
    return clnt_fd;
}

int Socket::get_fd()
{
    return _fd;
}

ssize_t Socket::read(std::vector<char>& buffer, int nbytes)
{
    buffer.reserve(nbytes);
    buffer.erase(buffer.begin(), buffer.end());
    int s = ::read(this->_fd, buffer.data(), nbytes);
    buffer[s] = 0;
    buffer.erase(buffer.begin() + s, buffer.end());
    return s;
}

ssize_t Socket::write(const std::vector<char>& buffer)
{
    auto len = ::write(this->_fd, buffer.data(), buffer.size());
    return len;
}

void Socket::close()
{
    if (_fd != -1)
    {
        ::close(_fd);
        _fd = -1;
    }
}
