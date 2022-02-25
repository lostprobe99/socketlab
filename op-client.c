#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>

#include "common.h"

#define BUF_SIUZE 1024

int main(int argc, char ** argv)
{
    if(argc != 3)
    {
        printf("Usage: %s <ip> <port>\n", argv[0]);
        return -1;
    }
    WSADATA wsaData;
    SOCKET hSocket;
    SOCKADDR_IN servAddr;
    char buf[BUF_SIUZE];
    if(WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
        ERROR_HANDLING("WSAStartup");
    
    hSocket = socket(AF_INET, SOCK_STREAM, 0);
    if(hSocket == INVALID_SOCKET)
        ERROR_HANDLING("socket");
    memset(&servAddr, 0, sizeof(servAddr));
    servAddr.sin_family = AF_INET;
    servAddr.sin_addr.s_addr = inet_addr(argv[1]);
    if(!strcmp(argv[1], "localhost"))
        servAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    servAddr.sin_port = htons(atoi(argv[2]));

    if(connect(hSocket, (struct sockaddr*)&servAddr, sizeof(servAddr)) == SOCKET_ERROR)
        ERROR_HANDLING("connect");
    else
        printf("Connected\n");
    
    char msg[BUF_SIUZE];
    int opndCnt, result;
    printf("Operand count: ");
    scanf("%d", &opndCnt);
    msg[0] = (char)opndCnt;

    for(int i = 0; i < opndCnt; i++)
    {
        printf("Operand %d: ", i + 1);
        scanf("%d", (int*)&msg[i * sizeof(int) + 1]);
    }

    printf("Operator: ");
    scanf(" %c", &msg[opndCnt * sizeof(int) + 1]);
    send(hSocket, msg, opndCnt * sizeof(int) + 2, 0);
    recv(hSocket, (char*)&result, sizeof(int), 0);
    printf("Operation result: %d\n", result);

    closesocket(hSocket);
    WSACleanup();
    
    return 0;
}