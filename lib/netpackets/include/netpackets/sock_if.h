/**
 * sock_if.h
 *
 * @author : lostprobe
 * @date   : 2025/05/24 22:15
 * @brief  : sock_if.h
 */
#ifndef NETPACKETS_SOCK_IF_H
#define NETPACKETS_SOCK_IF_H

#include "common.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief 通过 `ioctl` 获取指定网络接口的硬件地址(MAC地址)
 * 
 * @param itf 网络接口名称 (如 `eth0`)
 * @param addr 一个 u8 数组, 长度至少为 6 字节
 * 
 * @return 成功返回0，失败返回负数
 */
int get_itf_hwaddr(const char *itf, uint8_t *addr);

/**
 * @brief 通过 `ioctl` 获取指定网络接口的IP地址
 * 
 * @param itf 网络接口名称 (如 `eth0`)
 * @param addr 一个 u32 变量的指针
 * 
 * @return 成功返回0，失败返回负数
 */
int get_itf_ipaddr(const char *itf, uint32_t *addr);

/// @brief 绑定 socket 和 网卡
/// @return bind() 函数的返回值
int bind_itf(int sock_fd, const char *itf);

#ifdef __cplusplus
}
#endif

#endif // SOCK_IF_H