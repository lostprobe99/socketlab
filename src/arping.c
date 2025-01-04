/**
 * arping.c
 *
 * @author : lostprobe
 * @date   : 2024/10/13 16:39
 * @brief  : send arp request and receive arp reply
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <strings.h>
#include <unistd.h>

#include <sys/socket.h>
#include <sys/ioctl.h>
#include <sys/time.h>   // for struct timeval
#include <arpa/inet.h>

#include <net/if.h>     // for if_nametoindex
#include <linux/if.h>   // for struct ifreq
#include <linux/if_packet.h>    // for struct sockaddr_ll
#include <linux/if_arp.h>

#include "arp.h"

#define DEV "eth0"

int main() {

    /*
     * TODO: arping [--saddr <ip> | --taddr <ip> | --smac <mac> | --tmac <mac>]
     */

    int sock_fd = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ARP));
    uint8_t buffer[60];
    bzero(buffer, sizeof(buffer));
    int ifindex = 0;
    arp_msg_t arp_req = {0};
    struct sockaddr_ll sock_addr;
    struct ifreq ifr;
    struct timeval timeout = {
        .tv_sec = 1,
        .tv_usec = 0,
    };

    if(setsockopt(sock_fd, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout)) == -1)
    {
        perror("setsockopt()");
    }
    strncpy(ifr.ifr_name, DEV, sizeof(ifr.ifr_name));
    ifindex = if_nametoindex(DEV);
    printf("网卡索引: %d\n", ifindex);

    // 获取网卡 MAC
    if(ioctl(sock_fd, SIOCGIFHWADDR, &ifr) == -1)
    {
        perror("SIOCGIFHWADDR");
        return -3;
    }
    printf("网卡MAC: %02X:%02X:%02X:%02X:%02X:%02X\n", ALL_MAC_BYTE((uint8_t)ifr.ifr_hwaddr.sa_data));

    // sockaddr MAC 为本机 MAC
    memcpy(sock_addr.sll_addr, ifr.ifr_hwaddr.sa_data, sizeof(sock_addr.sll_addr));

    // 填充 sockaddr_ll
    sock_addr.sll_family = AF_PACKET;
    // sock_addr.sll_protocol = htons(ETH_P_ARP);
    sock_addr.sll_ifindex = ifindex;
    // sock_addr.sll_hatype = htons(ARPHRD_ETHER);
    // sock_addr.sll_halen = ETH_ALEN;

arping1(DEV, "192.168.5.4");

    if(bind(sock_fd, (struct sockaddr*)&sock_addr, sizeof(sock_addr)) == -1)
    {
        perror("bind()");
        return -5;
    }

    struct sockaddr_ll sender;
    socklen_t sender_len = sizeof(sender);
    bzero(&buffer, sizeof(buffer));
    int n = recvfrom(sock_fd, &arp_req, sizeof(arp_req), 0, (struct sockaddr*)&sender, &sender_len);
    if(n == -1 && errno == EAGAIN)
    {
        printf("recvfrom timeout %lds\n", timeout.tv_sec);
    }
    else if(n == -1)
    {
        perror("recvfrom()");
        return -6;
    }

    hexdump((uint8_t*)&arp_req, sizeof(arp_req));
    printf("arp_req.hardware_type = [%u]\n", ntohs(arp_req.hardware_type));   // 非单字节要转换字节序
    printf("arp_req.protocol_type = [%#x]\n",ntohs(arp_req.protocol_type));
    printf("arp_req.hardware_size = [%u]\n", arp_req.hardware_size);
    printf("arp_req.protocol_size = [%u]\n", arp_req.protocol_size);
    printf("arp_req.opcode = [%u]\n", ntohs(arp_req.opcode));
    printf("arp_req.sender_mac = [%02X:%02X:%02X:%02X:%02X:%02X]\n", ALL_MAC_BYTE(arp_req.sender_mac));
    printf("arp_req.sender_ip = [%d.%d.%d.%d]\n", ALL_IP_BYTE(arp_req.sender_ip));
    printf("arp_req.target_mac = [%02X:%02X:%02X:%02X:%02X:%02X]\n", ALL_MAC_BYTE(arp_req.target_mac));
    printf("arp_req.target_ip = [%d.%d.%d.%d]\n", ALL_IP_BYTE(arp_req.target_ip));

    close(sock_fd);

    return 0;
}