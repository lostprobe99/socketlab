#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include "common.h"

#define BUF_SIZE 1024

int main(int argc, char ** argv)
{
    if(argc != 3)
    {
        printf("Usage: %s <ip> <port>\n", argv[0]);
        exit(1);
    }
    int sock, opnd_cnt;
    char opmsg[BUF_SIZE];
    if((sock = socket(AF_INET, SOCK_STREAM, 0)) == -1)
        ERROR_HANDLING("socket");
    struct sockaddr_in serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(atoi(argv[2]));
    if(!strcmp(argv[1], "localhost"))
        serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    else
        serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
    if(connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1)
        ERROR_HANDLING("connect");
    else
        puts("Connected...");

    // input
    printf("Operand count: ");
    scanf("%d", &opnd_cnt);
    opmsg[0] = (char)opnd_cnt;

    for(int i = 0; i < opnd_cnt; i++)
    {
        printf("Operand %d: ", i + 1);
        scanf("%d", (int*)(opmsg + 1 + i * sizeof(int)));
    }
    printf("Operator: ");
    scanf(" %c", opmsg + opnd_cnt * sizeof(int) + 1);
    write(sock, opmsg, opnd_cnt * sizeof(int) + 2);

    int result;
    read(sock, &result, sizeof(result));

    printf("Operation result: %d\n", result);
    close(sock);
    
    return 0;
}
