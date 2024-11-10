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

int get_mac(const char *itf, struct sockaddr_ll *addr);

int get_ip4(const char *itf, struct sockaddr_in *addr);

int get_subnet_mask(const char *itf, struct sockaddr_in *addr);

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