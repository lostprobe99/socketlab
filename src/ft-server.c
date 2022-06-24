#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "common.h"

#define BUF_SIZE 1024

int32_t get_file_size(const char * filename)
{
#if defined(_MSC_VER) || defined(_MSC_EXTENSIONS) || defined(_WIN32)
    if(_access(filename, 04) == 0)  // 可读
        return _filelength(fileno(fopen(filename, "rb")));
#elif linux
    struct stat st;
    if(stat(filename, &st) == 0)
        return st.st_size;
#endif
    else
        return 0;
}

int main(int argc, char ** argv)
{
    if(argc != 2)
    {
        printf("Usage: %s <port>\n", argv[0]);
        return -1;
    }
    #if defined(_WIN32) || defined(_WIN64)
    WSADATA wsaData;
    if(WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
        ERROR_HANDLING("WSAStartup");
    #endif
    SOCKET hServerSocket = socket(AF_INET, SOCK_STREAM, 0), hClientSocket;
    if(hServerSocket == INVALID_SOCKET)
        ERROR_HANDLING("socket");
    SOCKADDR_IN servAddr, clntAddr;
    memset(&servAddr, 0, sizeof(servAddr));
    servAddr.sin_family = AF_INET;
    servAddr.sin_port = htons(atoi(argv[1]));
    servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    if(bind(hServerSocket, (SOCKADDR *)&servAddr, sizeof(servAddr)) == SOCKET_ERROR)
        ERROR_HANDLING("bind");
    if(listen(hServerSocket, 5) == SOCKET_ERROR)
        ERROR_HANDLING("listen");
    
    while(1)
    {
        hClientSocket = accept(hServerSocket, (SOCKADDR *)&clntAddr, NULL);
        // 接收文件名，并判断文件是否可读
        char msg[BUF_SIZE], buf[BUF_SIZE];
        memset(msg, 0, BUF_SIZE);
        memset(buf, 0, BUF_SIZE);
        recv(hClientSocket, buf, BUF_SIZE, 0);  // 接收文件名

        int32_t file_size = get_file_size(buf);
        // 判断文件是否可读
        if(file_size != 0)    // 文件大小不为 0
        {
            int32_t n = 0;
            send(hClientSocket, (char*)&file_size, 4, 0);     // 发送文件长度
            // 发送文件
            printf("send %s\n", buf);
            FILE *fp = fopen(buf, "rb");
            memset(buf, 0, BUF_SIZE);
            while((n = fread(buf, 1, BUF_SIZE, fp)) != 0)
                send(hClientSocket, buf, n, 0);
            fclose(fp);
        }
    }

    return 0;
}