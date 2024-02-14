#pragma once

#include "Socket.h"
#include "InetAddr.h"

class TcpSocket : public Socket {
private:
    InetAddr _addr;
public:
    TcpSocket();
    explicit TcpSocket(int);
    void listen(int n = 5);
    // void connect();
    virtual void open(const char *addr, uint16_t port) override;
    TcpSocket accept(InetAddr& );
    InetAddr addr() const;
    // virtual ssize_t read(std::vector<char>& , int) override;
    // virtual ssize_t write(const std::vector<char>&) override;
    // virtual void close() override;
};