/**
 * ether.h
 *
 * @author : lostprobe
 * @date   : 2025/01/04 13:41
 * @brief  : ether.h
 */
#ifndef NETPACKETS_ETHER_H
#define NETPACKETS_ETHER_H

#include "common.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct __packed _ethernet_hdr_
{
    /* 前导码 preamble: 7字节 0x55 */
    /* 帧起始界定符 SFD: 1字节 固定 0xd5 */
    /* 以太网帧头 */
    /* +------+------+------+ */
    /* | 目地AC | 源MAC | 协议类型/长度 | */
    /* +------+------+------+ */
    /* | 6字节  | 6字节  | 2字节  | */
    /* +------+------+------+ */
    uint8_t dst_mac[MAC_BYTE_LEN];    /* 目的MAC, 全 1 为广播地址 */
    uint8_t source_mac[MAC_BYTE_LEN];     /* 源MAC */
    uint16_t proto_type;    /* 协议类型, 小于 1536(0x0600) 表示数据长度, 否则表示上层协议号 */
    /* 以太网数据: 长度最小 46 字节, 最大 1500 字节 */
    /* 帧校验序列FCS: 4字节 CRC 校验码, 校验从以太网帧头开始, 不包含前导码和帧起始界定符 */
} ether_hdr_t;  // 14 bytes

typedef struct __packed _ether_frame_ {
    ether_hdr_t hdr;
    uint8_t data[ETH_DATA_MAX_LEN];
} ether_frame_t;

/// @brief 构建以太网头
/// @param hdr 以太网头
/// @param dest_mac 目的 MAC
/// @param source_mac 源 MAC
/// @param proto_type 协议类型
void pack_ether_hdr(ether_hdr_t *hdr, uint8_t *dst_mac, uint8_t *source_mac, uint16_t proto_type);

int pack_ether_frame(ether_frame_t *frame, uint8_t *dst_mac, uint8_t *source_mac, uint16_t proto_type, uint8_t *data, int data_len);

int send_ether_frame(int sock_fd, uint8_t *dst_mac, uint8_t *source_mac, uint16_t proto_type, uint8_t *data, int data_len);


#ifdef __cplusplus
}
#endif

#endif // ETHER_H