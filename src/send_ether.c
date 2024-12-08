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

typedef struct __packed _ether_frame_ {
    ether_hdr_t hdr;
    uint8_t data[1500];
} ether_frame_t;

int pack_ether_frame(ether_frame_t *frame, uint8_t *dest_mac, uint8_t *source_mac, uint16_t proto_type, uint8_t *data, int data_len)
{
    pack_ether_hdr(&frame->hdr, dest_mac, source_mac, proto_type);
    memcpy(frame->data, data, data_len);
    return data_len + sizeof(ether_hdr_t);
}

int send_ether_frame(int sock_fd, const char *itf, uint8_t *dest_mac, uint8_t *source_mac, uint16_t proto_type, uint8_t *data, int data_len)
{
    ether_frame_t frame;
    int len = pack_ether_frame(&frame, dest_mac, source_mac, proto_type, data, data_len);

    return sendto(sock_fd, &frame, len, 0, NULL, 0);
}

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
    DIE_IF(send_ether_frame(sock_fd, itf, dest_mac, addr.sll_addr, proto, data, strlen(data)) < 0, "sendto");

    close(sock_fd);
    return 0;
}