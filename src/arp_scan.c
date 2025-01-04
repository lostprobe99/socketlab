/**
 * arp_scan.c
 *
 * @author : lostprobe
 * @date   : 2024/11/10 15:04
 * @brief  : ARP 扫描
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h> // for struct timeval

#include <net/if.h>     // for if_nametoindex
#include <linux/if.h>   // for struct ifreq
#include <linux/if_packet.h>    // for struct sockaddr_ll
#include <linux/if_arp.h>

#include "common.h"
#include "debug.h"
#include "util.h"

#define MAC_FMT "%02X:%02X:%02X:%02X:%02X:%02X"
#define IP_FMT "%d.%d.%d.%d"

uint32_t create_mask(uint32_t n)
{
    return ~((1U << (32 - n)) - 1);
}

int arp_filter_target_mac(arp_msg_t* arp_msg, void* params)
{
    uint8_t* tmac = params;
    return memcmp(arp_msg->target_mac, tmac, arp_msg->hardware_size) == 0;
}

int arp_filter_target_ip(arp_msg_t* arp_msg, void* params)
{
    uint32_t tip = *(uint32_t *)arp_msg->target_ip;
    return (*(uint32_t *)arp_msg->target_ip) == tip;
}

int arp_filter_sender_ip(arp_msg_t* arp_msg, void* params)
{
    return (*(uint32_t *)arp_msg->sender_ip) == *(uint32_t *)params;
}

typedef int (*filter_t)(arp_msg_t*, void*);
int arp_filter(arp_msg_t* arp_msg, filter_t filter, void* params)
{
    int ret = filter(arp_msg, params);
    return ret;
}

/*
 * arp_scan eth0 192.168.5.4/24
 */
int main(int argc, char ** argv)
{
    int sock_fd = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ARP));
    char *ip_str, *subnet_mask_str, *itf;
    int mask_num = 0;
    arp_msg_t arp_msg = {0};
    struct sockaddr_in v4 = {0};
    struct sockaddr_ll mac = {0};
    
    if(argc < 3)
    {
        printf("Usage: %s <itf> <ip>/<subnet_mask>\n", "arp_scan");
        return 0;
    }

    itf = argv[1];
    ip_str = strtok(argv[2], "/");
    subnet_mask_str = strtok(NULL, "/");
    mask_num = atoi(subnet_mask_str);

    get_itf_ip4(itf, &v4);
    get_itf_mac(itf, &mac);

    // 将字符串形式的ip地址转换到字节形式
    struct in_addr ip_addr;
    inet_aton(ip_str, &ip_addr);
    uint32_t src_ip = ip_addr.s_addr;

    // 转换子网掩码
    uint32_t subnet_mask = htonl(create_mask(mask_num));

    INFO("socket: %d\n", sock_fd);
    INFO("itf: %s\n", itf);
    INFO("ip: %s\n", ip_str);
    INFO("subnet mask: %s\n", inet_ntoa(*(struct in_addr *)&subnet_mask));

    if( bind_itf(sock_fd, itf) == -1 )
        DIE("无法绑定网卡 %s", itf);

    if( set_recv_timeout(sock_fd, 3, 0) == -1 )
        perror("set_recv_timeout()");

    // 使用ip和子网掩码计算网络段和主机段
    uint32_t network_ip = src_ip & subnet_mask;
    uint32_t host_num = ~subnet_mask;
    INFO("sub network: %s\n", inet_ntoa(*(struct in_addr *)&network_ip));
    INFO("host num: %u\n", htonl(host_num));

    // 计算广播地址
    uint32_t broadcast_ip = network_ip | (~subnet_mask);
    uint32_t target_ip = 0;
    struct sockaddr_ll sender_addr = {0};
    socklen_t sender_addr_len = sizeof(sender_addr);
    int n = 0;

    // 使用网络段和主机段计算出ip地址，并发送arp请求
    for (uint32_t i = 1; i < htonl(host_num); i++) {
        memset(&arp_msg, 0, sizeof(arp_msg));
        target_ip = network_ip | htonl(i);
        printf("==> send arp to %s\n", inet_ntoa(*(struct in_addr *)&target_ip));
        for(int j = 0; j < 3; j++)
        {
            // retry 3
            printf("try %d\n", j + 1);
            arping(sock_fd, itf, inet_ntoa(*(struct in_addr *)&target_ip));
            n = recvfrom(sock_fd, &arp_msg, sizeof(arp_msg), 0, (struct sockaddr*)&sender_addr, &sender_addr_len);
            if(n == -1 && errno == EAGAIN)
                continue;   // 超时
            else if(n == -1)
                perror("recvfrom()");
            else
            {
                /*
                 * ARP 回包检测: 
                 * 1. opcode 为响应
                 * 2. 目标 ip 为本机
                 * 3. 目标 mac 为本机
                 */
                if(arp_msg.opcode == htons(ARPOP_REPLY)
                    && arp_filter(&arp_msg, arp_filter_target_mac, mac.sll_addr)
                    && arp_filter(&arp_msg, arp_filter_target_ip, &v4.sin_addr.s_addr)
                    && arp_filter(&arp_msg, arp_filter_sender_ip, &target_ip)
                    )
                {
                    printf(IP_FMT " -> " MAC_FMT "\n", ALL_IP_BYTE(arp_msg.sender_ip), ALL_MAC_BYTE(arp_msg.sender_mac));
                    break;
                }
                else
                    continue;
            }
        }
    }

    close(sock_fd);

    return 0;
}