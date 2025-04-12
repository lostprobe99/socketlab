/**
 * arp.c
 *
 * @author : lostprobe
 * @date   : 2025/01/04 13:28
 * @brief  : arp.c
 */
#include <unistd.h>
#include "util.h"
#include "arp.h"

void pack_arp_msg(arp_msg_t *arp_msg, uint8_t *sender_mac, uint8_t *sender_ip, uint8_t *target_ip)
{
    uint8_t broadcast_mac[6] = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff};

    memset(arp_msg, 0, sizeof(arp_msg_t));

    // 以太网帧的目标 MAC 为广播MAC，全1
    // 以太网帧的源 MAC 为本机 MAC
    // 以太网帧的协议类型为 ARP
    pack_ether_hdr(&arp_msg->eth_hdr, broadcast_mac, sender_mac, ETH_P_ARP);

    // ARP包的硬件类型: Ethernet
    arp_msg->hardware_type = htons(0x01);
    // ARP包的协议类型: IPv4
    arp_msg->protocol_type = htons(ETH_P_IP);
    // ARP包的长度: 6
    arp_msg->hardware_size = ETH_ALEN;
    // ARP IP地址的长度: 4
    arp_msg->protocol_size = IPV4_LEN;
    // ARP类型: request
    arp_msg->opcode = htons(ARPOP_REQUEST);
    // ARP 请求源 MAC
    memcpy(&arp_msg->sender_mac, sender_mac, sizeof(arp_msg->sender_mac));
    // ARP 请求源 IP
    // inet_pton(AF_INET, "192.168.31.203", &arp.sender_ip); // 源协议地址
    memcpy(&arp_msg->sender_ip, sender_ip, sizeof(arp_msg->sender_ip));
    // ARP 请求的目标 MAC 为全0
    bzero(&arp_msg->target_mac, sizeof(arp_msg->target_mac));
    // ARP 请求的目标 IP
    memcpy(&arp_msg->target_ip, target_ip, sizeof(arp_msg->target_ip));
    // 目标 ip 和源 ip 相同时为 Announcement
    // Announcement 用于检查 ip 是否已被使用
    // memcpy(&arp.target_ip, &sa->sin_addr.s_addr, sizeof(arp.target_ip));
}

// 新增函数: 发送 ARP 包
int send_arp_pack(int sock_fd, const char *itf, arp_msg_t *arp_msg)
{
    int ifindex = 0;
    struct sockaddr_ll sock_addr;
    struct sockaddr_ll src_hwaddr;

    ifindex = if_nametoindex(itf);
    if (ifindex == 0) {
        perror("if_nametoindex()");
        return -1;
    }

    // 获取网卡 MAC
    get_itf_mac(itf, &src_hwaddr);
    if (src_hwaddr.sll_halen == 0) {
        perror("get_itf_mac()");
        return -2;
    }

    // 填充 sockaddr_ll
    sock_addr.sll_family = AF_PACKET;
    sock_addr.sll_protocol = htons(ETH_P_ARP);
    sock_addr.sll_ifindex = ifindex;
    sock_addr.sll_hatype = htons(ARPHRD_ETHER);
    sock_addr.sll_halen = ETH_ALEN;
    // sockaddr MAC 为本机 MAC
    memcpy(sock_addr.sll_addr, src_hwaddr.sll_addr, sizeof(sock_addr.sll_addr));

    // 发送 ARP
    if (sendto(sock_fd, arp_msg, sizeof(arp_msg_t), 0, (struct sockaddr*)&sock_addr, sizeof(sock_addr)) == -1) {
        perror("sendto()");
        return -4;
    }

    return 0;
}

int arping(int sock, const char *itf, char * target_ip)
{
    int ifindex = 0;
    arp_msg_t arp = {0};
    struct sockaddr_ll sock_addr;
    struct sockaddr_ll src_hwaddr;
    struct sockaddr_in src_ip4;
    uint8_t to_ip[4] = {0};

    inet_pton(AF_INET, target_ip, to_ip);

    ifindex = if_nametoindex(itf);
    // printf("网卡索引: %d\n", ifindex);

    // 获取网卡 MAC
    get_itf_mac(itf, &src_hwaddr);
    // printf("网卡MAC: %02X:%02X:%02X:%02X:%02X:%02X\n", ALL_MAC_BYTE((uint8_t)src_hwaddr.sll_addr));

    // 获取网卡 IP
    get_itf_ip4(itf, &src_ip4);
    // printf("网卡 IP: %s\n", inet_ntoa(src_ip4.sin_addr));

    pack_arp_msg(&arp, src_hwaddr.sll_addr, (uint8_t *)&src_ip4.sin_addr.s_addr, to_ip);

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
    if(sendto(sock, &arp, sizeof(arp), 0, (struct sockaddr*)&sock_addr, sizeof(sock_addr)) == -1)
    {
        perror("sendto()");
        return -4;
    }

    return 0;
}

int arping1(const char *itf, char * target_ip)
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
    int ret = 0;

    if(sock_fd == -1)
    {
        perror("socket()");
        return -1;
    }

    if( (ret = arping(sock_fd, itf, target_ip)))
    {
        close(sock_fd);
        return ret;
    }

    close(sock_fd);
    return 0;
}