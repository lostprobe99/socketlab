/**
 * obj-echo-server.cpp
 *
 * @author : lostprobe
 * @date   : 2023/06/18 09:22
 * @brief  : obj-echo-server.cpp
 */
#include <iostream>
#include <vector>
#include <memory>
#include <cerrno>
#include <cstring>
#include <unistd.h>
#include "simplesocket/InetAddr.h"
#include "simplesocket/Socket.h"

int main(int argc, char ** argv)
{
    Socket serv_sock = Socket::open();
    std::cout << serv_sock << '\n';
    InetAddr addr("localhost", 9990);
    std::cout << addr << '\n';

    serv_sock.bind(addr);
    serv_sock.listen();

    std::cout << "Server starting at " << addr.port() << "\n";

    Socket clnt_sock;
    int clnt_fd;
    InetAddr clnt_addr;
    std::vector<char> buffer;
    int s = 0;
    while (1)
    {
        // 新的连接建立后才会销毁上一个连接的socket
        clnt_sock = serv_sock.accept(clnt_addr);
        std::cout << "Got client: addr = " << clnt_addr << ", fd = " << clnt_sock << "\n";
        s = clnt_sock.read(buffer, 64);
        if(s > 0)
        {
            std::cout << buffer.data() << "\n";
            clnt_sock.write(buffer);
        }
        else if(s == 0)
        {
            std::cout << "Client = " << clnt_addr << " closed\n";
            std::cout << "errno = " << errno << ", msg = " << strerror(errno) << "\n";
        }
        else
        {
            std::cout << "Client = " << clnt_addr << " error in read\n";
            std::cout << "errno = " << errno << ", msg = " << strerror(errno) << "\n";
        }
    }

    return 0;
}