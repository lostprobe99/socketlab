#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include "common.h"
#include "debug.h"

#define BUF_SIZE 1024

int main(int argc, char ** argv)
{
    if(argc != 3)
    {
        printf("Usage: %s <ip> <port>\n", argv[0]);
        exit(1);
    }
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if(sock == -1)
        FATAL("socket");
    int str_len;
    char msg[BUF_SIZE];

    // write addr
    struct sockaddr_in serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    if(!strcmp(argv[1], "localhost"))
        serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    else
        serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
    serv_addr.sin_port = htons(atoi(argv[2]));

    // connect
    if(connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1)
        FATAL("connect");
    else
        puts("Connected...");

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
        str_len = write(sock, msg, str_len - 1);
        str_len = read(sock, msg, BUF_SIZE);
        msg[str_len] = 0;
        printf("Message from server: %s\n", msg);
    }
    close(sock);

    return 0;
}
