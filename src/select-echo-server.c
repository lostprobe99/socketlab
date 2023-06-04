/**
 * select-echo-server.c
 *
 * @author : lostprobe
 * @date   : 2023/06/02 21:56
 * @brief  : select-echo-server.c
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/select.h>
#include <sys/time.h>
#include <unistd.h>
#include "debug.h"
#include "common.h"

#define BUF_SIZE 64

int main(int argc, char ** argv)
{
    if(argc != 2)
    {
        printf("Usage: %s <port>\n", argv[0]);
        exit(1);
    }
    socket_t fd = server(atoi(argv[1]));
    Assert(fd != -1, "server() error");
    printf("Server Starting at %s...\n", argv[1]);
    struct timeval timeout;

    int max_fd, fd_num /* 发生事件的 fd 的数量 */;
    fd_set reads, look;
    FD_ZERO(&reads);
    FD_SET(fd, &reads); // 设置要监听的 fd
    max_fd = fd + 1;
    int clnt_fd;
    sockaddr_in clnt_adr;
    socklen_t clnt_sz = sizeof(clnt_adr);
    int str_len;
    char buf[BUF_SIZE];
    while(1)
    {
        look = reads;
        timeout.tv_sec = 5;
        timeout.tv_usec = 0;
        if((fd_num = select(max_fd, &look, NULL, NULL, &timeout)) == -1)
            break;
        if(fd_num == 0)
            continue;
        
        for(int i = 0; i < max_fd; i++)
        {
            if(FD_ISSET(i, &look))
            {
                if(i == fd) // 服务端 fd 上有事件
                {
                    clnt_fd = accept(fd, (sockaddr*)&clnt_adr, &clnt_sz);
                    FD_SET(clnt_fd, &reads);    // 将客户端 fd 加入监听
                    if(max_fd < clnt_fd + 1)
                        max_fd = clnt_fd + 1;
                    printf("Got client: fd = %d, addr = %s\n", clnt_fd, inet_ntoa(clnt_adr.sin_addr));
                }
                else
                {
                    memset(buf, 0, BUF_SIZE);
                    // 读其他 fd
                    str_len = read(i, buf, BUF_SIZE);
                    if(str_len == 0)    // 连接已关闭
                    {
                        FD_CLR(i, &reads);
                        close(i);
                        printf("closed client: %d\n", i);
                    }
                    else
                    {
                        printf("Got Message: %s\n", buf);
                        write(i, buf, str_len);
                    }
                }
            }
        }
    }
    close(fd);

    return 0;
}