#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "common.h"

#define BUF_SIZE 1024

int main(int argc, char **argv)
{
    const char * ip = argv[1];
    const char * port = argv[2];
    if (argc != 3)
    {
        #ifdef _DEBUG
            ip = "127.0.0.1";
            port = "9000";
        #else
        printf("Usage: %s <ip> <port>\n", argv[0]);
        return -1;
        #endif
    }
    if(!strcmp(ip, "localhost"))
        ip = "127.0.0.1";
    // 创建 socket 连接
    int hSocket = make_socket(AF_INET, SOCK_STREAM, 0);
    if (hSocket == INVALID_SOCKET)
        ERROR_HANDLING("socket");

    sockaddr_in servAddr = make_sockaddr(AF_INET, ip, atoi(port));

    if (connect(hSocket, (sockaddr *)&servAddr, sizeof(servAddr)) == SOCKET_ERROR)
        ERROR_HANDLING("connect");
    else
        printf("Connected\n");

    char fileName[BUF_SIZE], buf[BUF_SIZE];
    int n;
    while (1)
    {
        n = printf("File Name: ");
        scanf("%s", fileName);
        if(strcmp(fileName, "exit") == 0)
            break;

        strcpy(buf, fileName);
        send(hSocket, buf, n + 1, 0);
        int32_t file_size;
        recv(hSocket, (char*)&file_size, 4, 0); // 接收文件大小
        if(file_size != 0)    // 文件存在
        {
            printf("file size: %d\n", file_size);
            FILE *fp = fopen(fileName, "wb");
            memset(buf, 0, BUF_SIZE);
            int t = 0;
            while(t < file_size)
            {   // 接收数据并写文件
                n = recv(hSocket, buf, BUF_SIZE, 0);
                printf("received %d bytes\r", (t += n));
                fwrite(buf, 1, n, fp);
                memset(buf, 0, BUF_SIZE);
            }
            fclose(fp);
            printf("\nend receive\n");
        }
        else
            printf("File Not Found\n");
    }

    return 0;
}