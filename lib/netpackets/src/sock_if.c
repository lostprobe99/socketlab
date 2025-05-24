/**
 * sock_if.c
 *
 * @author : lostprobe
 * @date   : 2025/05/24 22:15
 * @brief  : sock_if.c
 */

#include <sys/ioctl.h>
#include "netpackets/sock_if.h"

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

int bind_itf(int sock_fd, const char *itf)
{
    struct sockaddr_ll sock_addr = {0};
    sock_addr.sll_family = AF_PACKET;
    sock_addr.sll_ifindex = if_nametoindex(itf);

    if (sock_addr.sll_ifindex == -1)
        return -1;

    return bind(sock_fd, (struct sockaddr *)&sock_addr, sizeof(sock_addr));
}