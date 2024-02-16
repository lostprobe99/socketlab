/**
 * uecho-server.c
 *
 * @author : lostprobe
 * @date   : 2024/02/16 21:51
 * @brief  : uecho-server.c
 */
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include "common.h"
#include "debug.h"

#define BUF_SIZE 64

int main(int argc, char const *argv[])
{
    argv[1] = "9000";
    socket_t sock = make_socket(AF_INET, SOCK_DGRAM, 0);
    sockaddr_in serv_addr = make_sockaddr(AF_INET, NULL, atoi(argv[1]));
    // udp server

    if(bind(sock, (sockaddr*)&serv_addr, sizeof(serv_addr)) == -1)
        FATAL("bind");

    char buffer[BUF_SIZE];
    sockaddr_in client_addr;
    socklen_t client_len = sizeof(client_addr);
    int len;
    while(1)
    {
        len = recvfrom(sock, buffer, BUF_SIZE, 0, (sockaddr*)&client_addr, &client_len);
        buffer[len] = '\0';
        printf("Received message: %s\n", buffer);
        // send buffer to client
        sendto(sock, buffer, len, 0, (sockaddr*)&client_addr, client_len);
    }

    close(sock);
    return 0;
}