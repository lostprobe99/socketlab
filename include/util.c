/**
 * util.c
 *
 * @author : lostprobe
 * @date   : 2023/06/16 18:22
 * @brief  : util.c
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <errno.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>
#include <sys/wait.h>

#include "util.h"

int round_two(int x)
{
    if (x == 0)
        return 1;
    if (x == 1)
        return 2;
    // x 是 2 的幂
    if ((x & (x - 1)) == 0)
        return x;
    int last;
    do
    {
        // 不断循环，保留最高位的 1
        last = x;
        x = x & (x - 1);
    } while (x != 0);

    // 最后左移 1
    return last << 1;
}

int os_exec(char *cmd, ...)
{
    char *args[16] = {NULL};
    char *arg = NULL;
    int i = 0, status = 0;
    va_list va;
    pid_t pid = -1, ret;

    // 解析参数
    args[0] = cmd;
    va_start(va, cmd);
    i = 1;
    while (i < 16)
    {
        arg = va_arg(va, char *);
        if (arg == NULL)
            break;
        args[i] = arg;
        i++;
    }
    if (arg != NULL)
    {
        printf("Error: too many arguments\n");
        va_end(va);
        return -1;
    }
    va_end(va);

    pid = fork();
    if (pid == 0)
    {
        if (execvp(args[0], &args[0]) == -1)
            _exit(127);
    }
    else if (pid > 0)
    {
        while ((ret = waitpid(pid, &status, 0)) == -1)
        {
            if (errno == EINTR)
            {
                status = -1;
                break;
            }
        }
    }
    else
    {
        status = -1;
    }

    return status;
}

uint64_t timestamp()
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return (uint64_t)tv.tv_sec * 1000 + (uint64_t)tv.tv_usec / 1000;
}

int hexdump(uint8_t *begin, int s)
{
    for (int i = 0; i < s; i++)
    {
        if (i % 16 == 0)
            printf("\n\t");
        printf("%02x ", begin[i]);
    }
    printf("\n");
    return 0;
}

int hexdump1(const char *title, uint8_t *begin, int s)
{
    printf("%s", title);
    for (int i = 0; i < s; i++)
    {
        if (i % 16 == 0)
            printf("\n\t");
        printf("%02x ", begin[i]);
    }
    printf("\n");
    return 0;
}

uint32_t reverse32(uint32_t x)
{
    /*
     * 第四个字节
     * x << 24;
     */
    /*
     * 第三个字节
     * x = (x << 8) & 0x00ff0000;
     */
    /*
     * 第二个字节
     * x = (x >> 8) & 0x0000ff00;
     */
    /*
     * 第一个字节
     * x >> 24;
     */
    return ((x << 24)
        | (x << 8) & 0x00ff0000
        | (x >> 8) & 0x0000ff00
        | x >> 24);
}

int get_itf_mac(const char *itf, struct sockaddr_ll *addr)
{
    struct ifreq ifr;
    int sock_fd = -1;
    struct sockaddr_ll *sa_ll = addr;

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

    memcpy(sa_ll->sll_addr, ifr.ifr_hwaddr.sa_data, ETH_ALEN);
    // printf("网卡MAC: %02X:%02X:%02X:%02X:%02X:%02X\n", ALL_MAC_BYTE((uint8_t)sa_ll->sll_addr));

    close(sock_fd);
    return 0;
}

int get_itf_ip4(const char *itf, struct sockaddr_in *addr)
{
    struct ifreq ifr;
    int sock_fd = -1;
    struct sockaddr_in *sa_ll = addr;

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
    memcpy(addr, &ifr.ifr_addr, sizeof(struct sockaddr));

    close(sock_fd);
    return 0;
}

int get_itf_subnet_mask(const char *itf, struct sockaddr_in *addr)
{
    struct ifreq ifr;
    int sock_fd = -1;

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
    if (ioctl(sock_fd, SIOCGIFNETMASK, &ifr) == -1)
    {
        close(sock_fd);
        perror("SIOCGIFNETMASK");
        return -3;
    }
    memcpy(addr, &ifr.ifr_addr, sizeof(struct sockaddr));

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