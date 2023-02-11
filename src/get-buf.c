/**
 * get-buf.c
 *
 * @author : lostprobe
 * @date : 2022/02/28 19:10
 * @bref: 查看 socket 缓冲区大小
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "common.h"
#include "debug.h"

int main(int argc, char ** argv)
{
    #if defined(_WIN32) || defined(_WIN64)
    WSADATA wsaData;
    if(WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
       FATAL_EXIT("WSAStartup");
    #endif
    int snd_buf_size = 0, rcv_buf_size = 0;
    int len = sizeof(snd_buf_size), state;
    socket_t sock = socket(AF_INET, SOCK_STREAM, 0);
    // 获取发送缓冲区大小
    state = getsockopt(sock, SOL_SOCKET, SO_SNDBUF, (void*)&snd_buf_size, &len);
    if(state)
        FATAL_EXIT("getsockopt");
    printf("snd_buf_size: %d\n", snd_buf_size);

    // 获取接收缓冲区大小
    state = getsockopt(sock, SOL_SOCKET, SO_RCVBUF, (void*)&rcv_buf_size, &len);
    if(state)
        FATAL_EXIT("getsockopt");
    printf("rcv_buf_size: %d\n", rcv_buf_size);

    /* 
        option = 1;
        允许将 Time-Wait 状态下的端口号重新分配给新的 socket
        setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, (void *)&option, sizeof(option));
     */

    return 0;
}