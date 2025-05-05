/**
 * helper.c
 *
 * @author : lostprobe
 * @date   : 2025/05/05 13:30
 * @brief  : helper.c
 */
#include <sys/ioctl.h>
#include "netpackets/common.h"
#include "netpackets/helper.h"

int get_itf_hwaddr(const char *itf, uint8_t *addr)
{
    struct ifreq ifr;
    int sock_fd = -1;

    if (addr == NULL)
    {
        return -1;
    }

    // a socket, any type is ok
    sock_fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock_fd == -1)
    {
        perror("socket()");
        return -2;
    }

    memset(&ifr, 0, sizeof(ifr));
    strncpy(ifr.ifr_name, itf, sizeof(ifr.ifr_name));

    // 获取网卡 MAC
    if (ioctl(sock_fd, SIOCGIFHWADDR, &ifr) == -1)
    {
        close(sock_fd);
        perror("SIOCGIFHWADDR");
        return -3;
    }

    memcpy(addr, ifr.ifr_hwaddr.sa_data, MAC_BYTE_LEN);
    // printf("网卡MAC: %02X:%02X:%02X:%02X:%02X:%02X\n", ALL_MAC_BYTE((uint8_t)sa_ll->sll_addr));

    close(sock_fd);
    return 0;
}

int get_itf_ipaddr(const char *itf, uint32_t *addr)
{
    struct ifreq ifr;
    int sock_fd = -1;
    struct sockaddr_in sa_in = {0};

    if (addr == NULL)
    {
        return -1;
    }

    sock_fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock_fd == -1)
    {
        perror("socket()");
        return -2;
    }

    memset(&ifr, 0, sizeof(ifr));
    strncpy(ifr.ifr_name, itf, sizeof(ifr.ifr_name));

    // 获取网卡 IP
    if (ioctl(sock_fd, SIOCGIFADDR, &ifr) == -1)
    {
        close(sock_fd);
        perror("SIOCGIFADDR");
        return -3;
    }
    memcpy(&sa_in, &ifr.ifr_addr, sizeof(struct sockaddr));
    *addr = sa_in.sin_addr.s_addr;

    close(sock_fd);
    return 0;
}

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

int bind_itf(int sock_fd, const char *itf)
{
    struct sockaddr_ll sock_addr = {0};
    sock_addr.sll_family = AF_PACKET;
    sock_addr.sll_ifindex = if_nametoindex(itf);

    if (sock_addr.sll_ifindex == -1)
        return -1;

    return bind(sock_fd, (struct sockaddr *)&sock_addr, sizeof(sock_addr));
}