#pragma once

#include <string>
#include <vector>
#include <memory>

// 前置声明
class InetAddr;

class Socket
{
private:
    friend std::string to_string(const Socket&);
    friend std::ostream& operator<<(std::ostream& os, const Socket&);
    int _fd;
public:
    Socket();
    Socket(int);
    Socket(const Socket&);
    ~Socket();

    Socket& operator=(const Socket&);

    static Socket open();

    void bind(const InetAddr&);
    void listen();
    void setnonblocking();

    // Socket accept(InetAddr& );
    int accept(InetAddr& );
    ssize_t read(std::vector<char>& );
    ssize_t write(const std::vector<char>&);

    int get_fd();
};