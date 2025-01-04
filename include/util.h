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
#define __packed __attribute__((packed))

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



#ifdef __cplusplus
extern "C" {
#endif

/// @brief 构建以太网头
/// @param hdr 以太网头
/// @param dest_mac 目的 MAC
/// @param source_mac 源 MAC
/// @param proto_type 协议类型
void pack_ether_hdr(ether_hdr_t *hdr, uint8_t *dest_mac, uint8_t *source_mac, uint16_t proto_type);

int get_itf_mac(const char *itf, struct sockaddr_ll *addr);

int get_itf_ip4(const char *itf, struct sockaddr_in *addr);

int get_itf_subnet_mask(const char *itf, struct sockaddr_in *addr);

/// @brief  将字符串格式的 MAC 转换为字节数组，分隔符为 ':'
/// @param s 
/// @param mac 字节数组，最小为 6
/// @retval -1 失败
/// @retval  0 成功
int mac_aton(const char *s, uint8_t *mac);

/// @brief 将字节数组格式的 MAC 转换为字符串格式，分隔符为 ':'
char* mac_ntoa(uint8_t *mac);

/// @brief 绑定 socket 和 网卡
/// @return bind() 函数的返回值
int bind_itf(int sock_fd, const char *itf);

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

#ifdef __cplusplus
}
#endif

#endif // UTIL_H