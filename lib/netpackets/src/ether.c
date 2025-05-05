/**
 * ether.c
 *
 * @author : lostprobe
 * @date   : 2025/01/04 13:41
 * @brief  : ether.c
 */

#include "netpackets/ether.h"

void pack_ether_hdr(ether_hdr_t *hdr, uint8_t *dst_mac, uint8_t *source_mac, uint16_t proto_type)
{
    memset(hdr, 0, sizeof(ether_hdr_t));
    memcpy(hdr->dst_mac, dst_mac, sizeof(hdr->dst_mac));
    memcpy(hdr->source_mac, source_mac, sizeof(hdr->source_mac));
    hdr->proto_type = htons(proto_type);
}

int pack_ether_frame(ether_frame_t *frame, uint8_t *dst_mac, uint8_t *source_mac, uint16_t proto_type, uint8_t *data, int data_len)
{
    pack_ether_hdr(&frame->hdr, dst_mac, source_mac, proto_type);
    memcpy(frame->data, data, data_len);
    return data_len + sizeof(ether_hdr_t);
}

int send_ether_frame(int sock_fd, uint8_t *dst_mac, uint8_t *source_mac, uint16_t proto_type, uint8_t *data, int data_len)
{
    ether_frame_t frame;
    int len = pack_ether_frame(&frame, dst_mac, source_mac, proto_type, data, data_len);

    return sendto(sock_fd, &frame, len, 0, NULL, 0);
}