#include "simplesocket/TcpSocket.h"
#include <iostream>
#include <vector>
#include <cstring>
#include <thread>

void session(TcpSocket clnt_sock, InetAddr clnt_addr)
{
    std::vector<char> buffer;
    int s = 0;
    while(1)
    {
        s = clnt_sock.read(buffer, 64);
        if(s > 0)
        {
            std::cout << buffer.data() << "\n";
            clnt_sock.write(buffer);
        }
        else if(s == 0)
        {
            std::cout << "Client = " << clnt_addr << " closed\n";
            std::cout << "errno = " << errno << ", msg = " << ::strerror(errno) << "\n";
            break;
        }
        else
        {
            std::cout << "Client = " << clnt_addr << " error in read\n";
            std::cout << "errno = " << errno << ", msg = " << strerror(errno) << "\n";
            break;
        }
    }
    clnt_sock.close();
}

int main()
{
    TcpSocket sock, clnt_sock;
    InetAddr clnt_addr;
    std::vector<char> buffer;
    int s = 0;
    sock.open("localhost", 9000);
    std::cout << "Server starting at " << sock.addr() << "\n";
    sock.listen();
    while(1)
    {
        clnt_sock = sock.accept(clnt_addr);
        std::cout << "Got client: addr = " << clnt_addr << ", fd = " << clnt_sock << "\n";
        std::thread worker(session, std::move(clnt_sock), clnt_addr);
        worker.detach();
    }

    return 0;
}