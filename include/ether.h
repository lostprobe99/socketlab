/**
 * ether.h
 *
 * @author : lostprobe
 * @date   : 2025/01/04 13:41
 * @brief  : ether.h
 */
#ifndef ETHER_H
#define ETHER_H

#include "util.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct __packed _ethernet_hdr_
{
    /* 以太网头 */
    /* +------+------+------+ */
    /* | 目地AC | 源MAC | 协议类型 | */
    /* +------+------+------+ */
    /* | 6字节  | 6字节  | 2字节  | */
    /* +------+------+------+ */
    uint8_t dest_mac[6];    /* 目的MAC */
    uint8_t source_mac[6];     /* 源MAC */
    uint16_t proto_type;    /* 协议类型 */
} ether_hdr_t;

typedef struct __packed _ether_frame_ {
    ether_hdr_t hdr;
    uint8_t data[1500];
} ether_frame_t;

/// @brief 构建以太网头
/// @param hdr 以太网头
/// @param dest_mac 目的 MAC
/// @param source_mac 源 MAC
/// @param proto_type 协议类型
void pack_ether_hdr(ether_hdr_t *hdr, uint8_t *dest_mac, uint8_t *source_mac, uint16_t proto_type);

int pack_ether_frame(ether_frame_t *frame, uint8_t *dest_mac, uint8_t *source_mac, uint16_t proto_type, uint8_t *data, int data_len);

int send_ether_frame(int sock_fd, const char *itf, uint8_t *dest_mac, uint8_t *source_mac, uint16_t proto_type, uint8_t *data, int data_len);


#ifdef __cplusplus
}
#endif

#endif // ETHER_H