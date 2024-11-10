/**
 * util.c
 *
 * @author : lostprobe
 * @date   : 2023/06/16 18:22
 * @brief  : util.c
 */

#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>

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

int systemf(const char * fmt, ...)
{
    int res = 0;
    va_list args;
    va_start(args, fmt);
    int len = vsnprintf(NULL, 0, fmt, args);
    if(len <= 0)
    {
        va_end(args);
        perror("Error estimating buffer size");
        return -1;
    }
    char * cmd = malloc(len + 1);
    if(!cmd)
    {
        va_end(args);
        perror("Memory alloction failed");
        return -1;
    }

    va_start(args, fmt);
    vsnprintf(cmd, len + 1, fmt, args);
    va_end(args);

    res = system(cmd);
    free(cmd);

    return res;
}

uint64_t timestamp()
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return (uint64_t)tv.tv_sec * 1000 + (uint64_t)tv.tv_usec / 1000;
}

int hexdump(uint8_t *begin, int s)
{
    for(int i = 0; i < s; i++)
    {
        if(i % 16 == 0)
            printf("\n\t");
        printf("%02x ", begin[i]);
    }
    printf("\n");
    return 0;
}

int hexdump1(const char * title, uint8_t *begin, int s)
{
    printf("%s", title);
    for(int i = 0; i < s; i++)
    {
        if(i % 16 == 0)
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

int get_mac(const char *itf, struct sockaddr_ll *addr)
{
    struct ifreq ifr;
    int sock_fd = -1;
    struct sockaddr_ll *sa_ll = addr;

    if(addr == NULL)
    {
        return -1;
    }

    // a socket, any type is ok
    sock_fd = socket(AF_INET, SOCK_DGRAM, 0);
    if(sock_fd == -1)
    {
        perror("socket()");
        return -2;
    }

    memset(&ifr, 0, sizeof(ifr));
    strncpy(ifr.ifr_name, itf, sizeof(ifr.ifr_name));
    
    // 获取网卡 MAC
    if(ioctl(sock_fd, SIOCGIFHWADDR, &ifr) == -1)
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

int get_ip4(const char *itf, struct sockaddr_in *addr)
{
    struct ifreq ifr;
    int sock_fd = -1;
    struct sockaddr_in *sa_ll = addr;

    if(addr == NULL)
    {
        return -1;
    }

    sock_fd = socket(AF_INET, SOCK_DGRAM, 0);
    if(sock_fd == -1)
    {
        perror("socket()");
        return -2;
    }

    memset(&ifr, 0, sizeof(ifr));
    strncpy(ifr.ifr_name, itf, sizeof(ifr.ifr_name));
    
    // 获取网卡 IP
    if(ioctl(sock_fd, SIOCGIFADDR, &ifr) == -1)
    {
        close(sock_fd);
        perror("SIOCGIFADDR");
        return -3;
    }
    memcpy(addr, &ifr.ifr_addr, sizeof(struct sockaddr));

    close(sock_fd);
    return 0;
}

int get_subnet_mask(const char *itf, struct sockaddr_in *addr)
{
    struct ifreq ifr;
    int sock_fd = -1;

    if(addr == NULL)
    {
        return -1;
    }

    sock_fd = socket(AF_INET, SOCK_DGRAM, 0);
    if(sock_fd == -1)
    {
        perror("socket()");
        return -2;
    }

    memset(&ifr, 0, sizeof(ifr));
    strncpy(ifr.ifr_name, itf, sizeof(ifr.ifr_name));
    
    // 获取网卡 IP
    if(ioctl(sock_fd, SIOCGIFNETMASK, &ifr) == -1)
    {
        close(sock_fd);
        perror("SIOCGIFNETMASK");
        return -3;
    }
    memcpy(addr, &ifr.ifr_addr, sizeof(struct sockaddr));

    close(sock_fd);
    return 0;
}

int arping(const char *itf, char * target_ip)
{
    /**
     *** __domain
     * PF_PACKET 二层协议簇
     *** __type
     * 指定 PF_PACKET 后，第二参数只能选择 SOCK_RAW 或 SOCK_DGRAM
     * SOCK_RAW可以自己构造帧头，SOCK_DGRAM不能
     * 帧头通过 sockaddr_ll 创建
     *** __protocol
     * ETH_P_ARP 表示只接收 ARP
     * ETH_P_ALL 表示接收所有帧
     */
    int sock_fd = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ARP));
    int ifindex = 0;
    arp_msg_t arp = {0};
    struct sockaddr_ll sock_addr;
    struct sockaddr_ll src_hwaddr;
    struct sockaddr_in src_ip4;

    if(sock_fd == -1)
    {
        perror("socket()");
        return -1;
    }

    ifindex = if_nametoindex(itf);
    printf("网卡索引: %d\n", ifindex);

    // 获取网卡 MAC
    get_mac(itf, &src_hwaddr);
    printf("网卡MAC: %02X:%02X:%02X:%02X:%02X:%02X\n", ALL_MAC_BYTE((uint8_t)src_hwaddr.sll_addr));

    // 获取网卡 IP
    get_ip4(itf, &src_ip4);
    printf("网卡 IP: %s\n", inet_ntoa(src_ip4.sin_addr));

    // 以太网帧的目标 MAC 为广播MAC，全1
    // memcpy(arp.dest_mac, broadcast_mac, sizeof(arp.dest_mac));
    memset(arp.dest_mac, 0xff, sizeof(arp.dest_mac));
    // 以太网帧的源 MAC 为本机 MAC
    memcpy(arp.source_mac, src_hwaddr.sll_addr, sizeof(arp.source_mac));
    // 以太网帧的协议类型
    arp.proto_type = htons(ETH_P_ARP);

    // ARP包的硬件类型: Ethernet
    arp.hardware_type = htons(0x01);
    // ARP包的协议类型: IPv4
    arp.protocol_type = htons(ETH_P_IP);
    // ARP包的长度: 6
    arp.hardware_size = ETH_ALEN;
    // ARP IP地址的长度: 4
    arp.protocol_size = IPV4_LEN;
    // ARP类型: request
    arp.opcode = htons(ARPOP_REQUEST);
    // ARP 请求源 MAC 为 itf 的 MAC
    memcpy(&arp.sender_mac, src_hwaddr.sll_addr, sizeof(arp.sender_mac));
    // ARP 请求源 IP 为 itf 的 IP
    // inet_pton(AF_INET, "192.168.31.203", &arp.sender_ip); // 源协议地址
    memcpy(&arp.sender_ip, &src_ip4.sin_addr.s_addr, sizeof(arp.sender_ip));
    // ARP 请求的目标 MAC 为全0
    bzero(&arp.target_mac, sizeof(arp.target_mac));
    // ARP 请求的目标 IP
    inet_pton(AF_INET, target_ip, &arp.target_ip); // 目标协议地址
    // 目标 ip 和源 ip 相同时为 Announcement
    // Announcement 用于检查 ip 是否已被使用
    // memcpy(&arp.target_ip, &sa->sin_addr.s_addr, sizeof(arp.target_ip));

    // 填充 sockaddr_ll
    sock_addr.sll_family = AF_PACKET;
    sock_addr.sll_protocol = htons(ETH_P_ARP);
    sock_addr.sll_ifindex = ifindex;
    sock_addr.sll_hatype = htons(ARPHRD_ETHER);
    sock_addr.sll_halen = ETH_ALEN;
    // sockaddr MAC 为本机 MAC
    memcpy(sock_addr.sll_addr, src_hwaddr.sll_addr, sizeof(sock_addr.sll_addr));

    // printf("发送到: %02X:%02X:%02X:%02X:%02X:%02X\n", ALL_MAC_BYTE(sock_addr.sll_addr));
    // hexdump((uint8_t*)&arp, sizeof(arp));

    // 发送 ARP
    if(sendto(sock_fd, &arp, sizeof(arp), 0, (struct sockaddr*)&sock_addr, sizeof(sock_addr)) == -1)
    {
        perror("sendto()");
        return -4;
    }

    close(sock_fd);
    return 0;
}