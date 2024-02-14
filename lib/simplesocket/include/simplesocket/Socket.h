#pragma once

#include <string>
#include <vector>
#include <memory>

enum class SocketType {
    TCP,
    UDP,
    UNIX
};

// 前置声明
class InetAddr;

class Socket
{
private:
    friend std::string to_string(const Socket&);
    friend std::ostream& operator<<(std::ostream& os, const Socket&);

protected:
    int _fd;
    int bindWrap(const InetAddr& addr);
    int acceptWrap(InetAddr& );

public:
    Socket();
    explicit Socket(int);
    Socket(const Socket&) = delete;
    Socket(Socket&& other) noexcept;
    ~Socket();

    Socket& operator=(const Socket&) = delete;
    Socket& operator=(Socket&&);

    virtual void open(const char *addr, uint16_t port) = 0;

    void setnonblocking();

    virtual ssize_t read(std::vector<char>& , int);
    virtual ssize_t write(const std::vector<char>&);
    virtual void close();

    int get_fd();
};