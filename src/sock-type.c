/**
 * sock-type.c
 *
 * @author : lostprobe
 * @date : 2022/02/28 19:10
 * @brief: 查看 socket 类型
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "common.h"
#include "debug.h"

int main(int argc, char ** argv)
{
    // 创建 socket
    int tcp_sock = make_socket(AF_INET, SOCK_STREAM, 0);
    int udp_sock = make_socket(AF_INET, SOCK_DGRAM, 0);
    int sock_type, state;
    int optlen = sizeof(sock_type);
    printf("SOCK_STREAM: %d\n", SOCK_STREAM);
    printf("SOCK_DGRAM: %d\n", SOCK_DGRAM);

    // 获取 socket 类型
    state = getsockopt(tcp_sock, SOL_SOCKET, SO_TYPE, (void*)&sock_type, &optlen);
    if(state)
        DIE("getsockopt");
    printf("tcp_sock type: %d\n", sock_type);

    state = getsockopt(udp_sock, SOL_SOCKET, SO_TYPE, (void*)&sock_type, &optlen);
    if(state)
        DIE("getsockopt");
    printf("udp_sock type: %d\n", sock_type);

    return 0;
}