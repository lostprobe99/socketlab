/**
 * arp.h
 *
 * @author : lostprobe
 * @date   : 2025/01/04 13:25
 * @brief  : arp.h
 */
#ifndef ARP_H
#define ARP_H

#include "util.h"
#include "ether.h"

#ifdef __cplusplus
extern "C" {
#endif

// ARP包结构
// 字段顺序不可更改
typedef struct __packed _arp_msg_
{
#define IPV4_LEN 4
    ether_hdr_t eth_hdr;

    /* arp头 */
    uint16_t hardware_type; // 硬件类型#1:Ethernet
    uint16_t protocol_type; // 协议类型#0x0800:IPv4
    uint8_t  hardware_size; // MAC地址长度#6
    uint8_t  protocol_size; // IP地址长度#4
    uint16_t opcode;        // ARP类型#1:request; 2:reply
    uint8_t  sender_mac[ETH_ALEN];  // 源MAC
    uint8_t  sender_ip[IPV4_LEN];   // 源IP地址
    uint8_t  target_mac[ETH_ALEN];  // 目标MAC
    uint8_t  target_ip[IPV4_LEN];   // 目标IP

    /* 18字节填充 */
    uint8_t padding[18];
}arp_msg_t;

/// @brief 构建 ARP 报文, 目的 MAC 填充为 广播 MAC
/// @param arp_msg ARP 报文
/// @param sender_mac 源 MAC
/// @param sender_ip 源 IP
/// @param target_ip 目标 IP
void pack_arp_msg(arp_msg_t *arp_msg, uint8_t *sender_mac, uint8_t *sender_ip, uint8_t *target_ip);

int arping(int sock, const char *itf, char * target_ip);
int arping1(const char *itf, char * target_ip);

#ifdef __cplusplus
}
#endif

#endif // ARP_H