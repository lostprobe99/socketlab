/**
 * uecho-client.c
 *
 * @author : lostprobe
 * @date   : 2024/02/16 21:47
 * @brief  : uecho-client.c
 */
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include "common.h"
#include "debug.h"

#define BUF_SIZE 64

int main(int argc, char const *argv[])
{
    argv[1] = "127.0.0.1";
    argv[2] = "9000";
    sockaddr_in peer_addr;
    socklen_t peer_adddr_sz = sizeof(peer_addr);
    int str_len;
    char msg[BUF_SIZE] = {0};
    socket_t sock = make_socket(AF_INET, SOCK_DGRAM, 0);
    sockaddr_in serv_addr = make_sockaddr(AF_INET, argv[1], atoi(argv[2]));

    while(1)
    {
        printf("Input message(Q to quit): ");
        fgets(msg, BUF_SIZE, stdin);
        str_len = strlen(msg);
        if(str_len - 1 == 0 && msg[0] == '\n')
            continue;
        msg[str_len - 1] = 0;
        if(!strcmp(msg, "q") || !strcmp(msg, "Q"))
            break;
        
        // udp write
        sendto(sock, msg, str_len, 0, (sockaddr*)&serv_addr, sizeof(serv_addr));

        str_len = recvfrom(sock, msg, sizeof(msg), 0, (sockaddr*)&peer_addr, &peer_adddr_sz);
        msg[str_len] = 0;
        printf("Message from server: %s\n", msg);
    }
    close(sock);

    return 0;
}