/**
 * broadcast.c
 *
 * @author : lostprobe
 * @date   : 2024/10/13 16:39
 * @brief  : broadcast demo
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include "common.h"
#include "debug.h"

int main(int argc, char ** argv)
{
    // send and recv broadcast message
    int fd = make_socket(AF_INET, SOCK_DGRAM, 0);

    // 设置广播
    // int optval = 1;
    // setsockopt(fd, SOL_SOCKET, SO_BROADCAST, &optval, sizeof(optval));
    set_broadcast(fd);
    INFO("fd: %d", fd);

    sockaddr_in addr = make_sockaddr(AF_INET, 0, 8888);
    addr.sin_addr.s_addr = htonl(INADDR_BROADCAST);

    // 发送广播信息
    // char * msg = "hello world";
    char *msg = argv[1];
    sendto(fd, msg, strlen(msg), 0, (sockaddr *)&addr, sizeof(addr));
    INFO("send broadcast message: %s", msg);
    close(fd);

    return 0;
}