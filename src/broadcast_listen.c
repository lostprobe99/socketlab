/**
 * recv_brodcast.c
 *
 * @author : lostprobe
 * @date   : 2024/11/10 14:45
 * @brief  : recevice broadcast msg
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include "common.h"
#include "debug.h"
#include "util.h"

int main(int argc, char ** argv)
{
    // 接受广播信息
    socket_t fd = make_socket(AF_INET, SOCK_DGRAM, 0);
    errif(fd == INVALID_SOCKET, "make socket error");
    set_broadcast(fd);
    sockaddr_in addr = make_sockaddr(AF_INET, 0, 8888);
    // 绑定套接字
    if (bind(fd, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        perror("Binding socket failed");
        exit(EXIT_FAILURE);
    }

    char buf[1024];
    while (1) {
        memset(buf, 0, sizeof(buf));
        socklen_t addr_len = sizeof(addr);
        int ret = recvfrom(fd, buf, sizeof(buf), 0, (sockaddr*)&addr, &addr_len);
        errif(ret == SOCKET_ERROR, "recvfrom error");
        hexdump1("recvfrom", buf, ret);
        printf("Received broadcast message: %s\n", buf);
    }
    close(fd);

    return 0;
}