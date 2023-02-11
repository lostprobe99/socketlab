#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include "common.h"
#include "debug.h"

#define BUF_SIZE 1024

int main(int argc, char ** argv)
{
    if(argc != 2)
    {
        printf("Usage: %s <port>\n", argv[0]);
        exit(1);
    }
    int serv_sock, clnt_sock;
    char msg[BUF_SIZE];

    struct sockaddr_in serv_addr, clnt_addr;
    socklen_t clnt_addr_sz = sizeof(clnt_addr);
    int str_len;

    if((serv_sock = socket(AF_INET, SOCK_STREAM, 0)) == -1)
        FATAL_EXIT("socket");

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(atoi(argv[1]));

    if((bind(serv_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr))) == -1)
        FATAL_EXIT("bind");

    if(listen(serv_sock, 5) == -1)
        FATAL_EXIT("listen");
    
    // 接受数据
    for(int i = 0; i < 5; i++)
    {
        clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_addr, &clnt_addr_sz);
        if(clnt_sock == -1)
            FATAL_EXIT("accept");
        else
            printf("Connected client %d\n", i + 1);
        while((str_len = read(clnt_sock, msg, BUF_SIZE)) != 0)
            write(clnt_sock, msg, str_len);
        close(clnt_sock);
    }
    close(serv_sock);

    return 0;
}
