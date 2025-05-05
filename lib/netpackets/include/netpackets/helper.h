/**
 * helper.h
 *
 * @author : lostprobe
 * @date   : 2025/05/05 13:24
 * @brief  : helper.h
 */

#ifndef NETPACKETS_HELPER_H
#define NETPACKETS_HELPER_H

#ifdef __cplusplus
extern "C" {
#endif

int get_itf_hwaddr(const char *itf, uint8_t *addr);

int get_itf_ipaddr(const char *itf, uint32_t *addr);

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

#ifdef __cplusplus
}
#endif

#endif // NETPACKETS_HELPER_H