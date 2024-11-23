/**
 * util.h
 *
 * @author : lostprobe
 * @date   : 2023/06/16 18:22
 * @brief  : util.h
 */
#ifndef UTIL_H
#define UTIL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include <sys/socket.h>
#include <sys/ioctl.h>
#include <arpa/inet.h>

#include <net/if.h>     // for if_nametoindex
#include <linux/if.h>   // for struct ifreq
#include <linux/if_packet.h>    // for struct sockaddr_ll
#include <linux/if_arp.h>

#define ALL_MAC_BYTE(x) x[0], x[1], x[2], x[3], x[4], x[5]
#define ALL_IP_BYTE(x) x[0], x[1], x[2], x[3]

typedef struct __attribute__((packed)) _ethernet_hdr_
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

// ARP包结构
// 字段顺序不可更改
typedef struct __attribute__((packed)) _arp_msg_
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


/// @brief 构建以太网头
/// @param hdr 以太网头
/// @param dest_mac 目的 MAC
/// @param source_mac 源 MAC
/// @param proto_type 协议类型
void pack_ether_hdr(ether_hdr_t *hdr, uint8_t *dest_mac, uint8_t *source_mac, uint16_t proto_type);

/// @brief 构建 ARP 报文, 目的 MAC 填充为 广播 MAC
/// @param arp_msg ARP 报文
/// @param sender_mac 源 MAC
/// @param sender_ip 源 IP
/// @param target_ip 目标 IP
void pack_arp_msg(arp_msg_t *arp_msg, uint8_t *sender_mac, uint8_t *sender_ip, uint8_t *target_ip);

int arping(const char *itf, char * target_ip);

int get_itf_mac(const char *itf, struct sockaddr_ll *addr);

int get_itf_ip4(const char *itf, struct sockaddr_in *addr);

int get_itf_subnet_mask(const char *itf, struct sockaddr_in *addr);

// 平台无关

/// @brief 返回大于 x 的最小 2 的幂
/// @param x 
/// @return 
int round_two(int x);

/// @brief 可接收格式化字符串的system()函数
/// @param fmt 格式字符串
/// @param ... 格式字符串的参数
/// @return system()函数的返回值
int systemf(const char * fmt, ...);

/// @brief 打印 begin ~ begin + s 的十六进制数据
int hexdump(uint8_t *begin, int s);

int hexdump1(const char * title, uint8_t *begin, int s);

/// @brief 逐位反转
/// @return 反转后的结果
uint32_t reverse32(uint32_t x);

#endif // UTIL_H