#pragma once

class InetAddr;

class Socket
{
private:
    /* data */
    int _fd;
public:
    Socket();
    Socket(int);
    ~Socket();

    void bind(InetAddr *);
    void listen();
    void setnonblocking();

    int accept(InetAddr *);

    int get_fd();
};