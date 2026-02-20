/**
 * send_ip.c
 *
 * @author : lostprobe
 * @date   : 2025/05/05 13:21
 * @brief  : send_ip.c
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "netpackets/ether.h"
#include "netpackets/ipv4.h"
#include "netpackets/helper.h"
#include "netpackets/sock_if.h"
#include "simple_log.h"

#define IPV4_DATA_MIN_LEN (ETH_DATA_MIN_LEN - sizeof(ipv4_hdr_t))



int config_ip4_hdr(ipv4_hdr_t *ip, uint16_t hdr_len, uint16_t data_len, uint32_t src_ip, uint32_t dst_ip)
{
    ip->version = 4;
    ip->ihl = hdr_len / 4;
    ip->tos = 0;
    ip->tot_len = htons(hdr_len + data_len);    // 多字节需要转换字节序
    ip->id = htons(getpid());
    /* ??? */
    // ip->flags |= 0x2;  // = 0b010, DF 不分片
    // ip->frag_off = htons(0);
    ip->frag |= DF;
    ip->frag = htons(ip->frag);
    ip->ttl = 64;
    ip->protocol = 40;
    ip->checksum = 0;
    ip->src_addr = src_ip;
    ip->dst_addr = dst_ip;
    ip->checksum = htons(ipv4_checksum(ip, sizeof(ipv4_hdr_t)));
}

int send_frame(const char *itf, uint8_t *frame, uint16_t len)
{
    int sock = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
    if(sock < 0)
    {
        log_error("create socket failed");
        return -1;
    }
    if(bind_itf(sock, itf) < 0)
    {
        log_error("bind itf failed\n");
        return -1;
    }
    if(sendto(sock, frame, len, 0, NULL, 0) < 0)
    {
        log_error("sendto failed\n");
        return -2;
    }
    return 0;
}

int main(int argc, char ** argv)
{
    set_log_level(LOG_LEVEL_DEBUG);
    const char *itf = "eth0";
    const char * dst_ip_str = "192.168.5.2";
    // uint8_t dst_mac[MAC_BYTE_LEN] = {0x80, 0x2d, 0x1a, 0x1e, 0x55, 0xde};
    uint8_t dst_mac[MAC_BYTE_LEN] = {0x00, 0x11, 0x22, 0x33, 0x44, 0x55};
    uint8_t src_mac[MAC_BYTE_LEN] = {0};
    uint32_t src_ip = 0;
    char data[IPV4_DATA_MIN_LEN] = "hello world";

    uint8_t frame[sizeof(ether_hdr_t) + sizeof(ipv4_hdr_t) + sizeof(data)] = {0};
    ether_hdr_t *eth = (ether_hdr_t *)frame;
    ipv4_hdr_t *ip = (ipv4_hdr_t *)(frame + sizeof(ether_hdr_t));
    uint8_t *payload = (uint8_t *)(frame + sizeof(ether_hdr_t) + sizeof(ipv4_hdr_t));

    get_itf_ipaddr(itf, &src_ip);
    get_itf_hwaddr(itf, (uint8_t *)&src_mac);

    // 组装 ether 头
    pack_ether_hdr(eth, dst_mac, src_mac, ETH_TYPE_IPV4);
    log_debug_hexdump("eth hdr", (uint8_t *)eth, sizeof(ether_hdr_t));

    // 配置 ip 头
    config_ip4_hdr(ip, sizeof(ipv4_hdr_t), sizeof(data), src_ip, inet_addr(dst_ip_str));
    log_debug_hexdump("ip hdr", (uint8_t *)ip, sizeof(ipv4_hdr_t));

    memcpy(payload, data, sizeof(data));
    log_debug_hexdump("frame", frame, sizeof(frame));

    if(send_frame(itf, frame, sizeof(frame)) < 0)
    {
        log_error("send frame failed\n");
        return -1;
    }

    return 0;
}