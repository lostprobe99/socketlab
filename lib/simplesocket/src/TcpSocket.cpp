#include "TcpSocket.h"
#include <sys/socket.h>
#include <cstring>
#include <stdexcept>

TcpSocket::TcpSocket(): Socket()
{

}
TcpSocket::TcpSocket(int fd) : Socket(fd)
{}

void TcpSocket::open(const char *addr, uint16_t port)
{
    _fd = socket(AF_INET, SOCK_STREAM, 0);
    if(this->_fd == -1)
    {
        throw std::runtime_error("Failed to create TCP socket: " + std::string(std::strerror(errno)));
    }
    // 设置 SO_REUSEADDR
    int sock_opt = 1, optlen = sizeof(sock_opt);
    setsockopt(_fd, SOL_SOCKET, SO_REUSEADDR, (void*)&sock_opt, optlen);
    _addr.af(AF_INET);
    _addr.addr(addr);
    _addr.port(port);
    if(bindWrap(_addr) == -1)
    {
        throw std::runtime_error("Failed to bind addr: " + std::string(std::strerror(errno)));
    }
}

void TcpSocket::listen(int n)
{
    if(::listen(this->_fd, n) == -1)
    {
        throw std::runtime_error("Failed to listen: " + std::string(std::strerror(errno)));
    }
}

TcpSocket TcpSocket::accept(InetAddr& clnt_addr)
{
    int clnt_fd = acceptWrap(clnt_addr);
    if(clnt_fd == -1)
    {
        throw std::runtime_error("Failed to accept" + std::string(std::strerror(errno)));
    }
    return TcpSocket(clnt_fd);
}

InetAddr TcpSocket::addr() const
{
    return _addr;
}