#include "Socket.h"
#include "InetAddr.h"
#include "debug.h"

#include <unistd.h>
#include <fcntl.h>
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
    // _fd = socket(AF_INET, SOCK_STREAM, 0);
    // errif(_fd == -1, "socket create error");
}

Socket::Socket(int fd) : _fd(fd){
    errif(_fd == -1, "socket create error");
}

Socket::Socket(const Socket& rhs)
{
    this->_fd = rhs._fd;

}

Socket Socket::open()
{
    return Socket(socket(AF_INET, SOCK_STREAM, 0));
}

Socket::~Socket()
{
    if(_fd != -1)
    {
        close(_fd);
        _fd = -1;
    }
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

ssize_t Socket::read(std::vector<char>& buffer)
{
    buffer.clear();
    char c[1];
    int s = 0;
    do
    {
        s = ::read(this->_fd, c, 1);
        if(s == 0)
            break;
        else if(s < 0)
            return s;
        buffer.push_back(c[0]);
    } while (s > 0);
    
    // while((s = ::read(this->_fd, c, 1)) > 0)
    // {
    //     buffer.push_back(c[0]);
    // }
    return buffer.size();
}

ssize_t Socket::write(const std::vector<char>& buffer)
{
    auto len = ::write(this->_fd, buffer.data(), buffer.size());
    return len;
}
