/**
 * helper.c
 *
 * @author : lostprobe
 * @date   : 2025/05/05 13:30
 * @brief  : helper.c
 */
#include "netpackets/common.h"
#include "netpackets/helper.h"

int mac_aton(const char *s, uint8_t *mac)
{
    /*
     * %hhx: %x - 十六进制
     * hh: char 或 unsigned char
     */
    int ret = sscanf(s, "%hhx:%hhx:%hhx:%hhx:%hhx:%hhx", mac, mac + 1, mac + 2, mac + 3, mac + 4, mac + 5);
    return ret == 6 ? 0 : -1; // 不是 6 个则返回 -1
}

char *mac_ntoa(uint8_t *mac)
{
    static char s[32] = {0};
    snprintf(s, sizeof(s), "%02x:%02x:%02x:%02x:%02x:%02x", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
    return s;
}