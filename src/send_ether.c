/**
 * send_ether.c
 *
 * @author : lostprobe
 * @date   : 2024/12/07 22:18
 * @brief  : send_ether.c
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>

#include "debug.h"
#include "util.h"
#include "netpackets/ether.h"

int main(int argc, char ** argv)
{
    char * itf = argv[1];
    char * s_mac = argv[2];
    char * s_proto = argv[3];
    char * data = argv[4];

    Assert(argc == 5, "Usage: %s <itf> <dest_mac> <proto> <data>", argv[0]);

    int proto = 0;
    uint8_t dest_mac[ETH_ALEN] = {0};
    ether_frame_t frame;
    struct sockaddr_ll addr;
    int sock_fd = socket(AF_PACKET, SOCK_RAW, 0);

    DIE_IF(sock_fd < 0, "socket");

    sscanf(s_proto, "%x", &proto);
    if(mac_aton(s_mac, dest_mac) < 0)
        DIE("Bad destination mac address: %s\n", s_mac);

    INFO("\n\titf: [%s], dest mac: [%s], proto: [%#x]\n\tdata: [%s]\n", itf, mac_ntoa(dest_mac), proto, data);

    DIE_IF(bind_itf(sock_fd, itf) < 0, "bind");
    DIE_IF(get_itf_mac(itf, &addr) < 0, "ioctl");
    INFO("Send [%s] to [%s]\n", data, s_mac);
    DIE_IF(send_ether_frame(sock_fd, dest_mac, addr.sll_addr, proto, data, strlen(data)) < 0, "sendto");

    close(sock_fd);
    return 0;
}