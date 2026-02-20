/**
 * ipv4.c
 *
 * @author : lostprobe
 * @date   : 2025/10/05 12:42
 * @brief  : ipv4.c
 */

#include "netpackets/ipv4.h"

void ipv4_ntohs(ipv4_hdr_t *ipv4_hdr)
{
    ipv4_hdr->tot_len = ntohs(ipv4_hdr->tot_len);
    ipv4_hdr->id = ntohs(ipv4_hdr->id);
    ipv4_hdr->checksum = ntohs(ipv4_hdr->checksum);
}

void ipv4_htons(ipv4_hdr_t *ipv4_hdr)
{
    ipv4_hdr->tot_len = htons(ipv4_hdr->tot_len);
    ipv4_hdr->id = htons(ipv4_hdr->id);
    ipv4_hdr->checksum = htons(ipv4_hdr->checksum);
}

/* rfc 1071 */ /* 计算校验和前要将校验和字段设置为0 */
uint16_t ipv4_checksum(void* data, uint16_t len)
{
    uint8_t *p = (uint8_t *)data;
    uint32_t sum = 0;
    uint16_t result = 0;
    for (; len > 1; len -= 2)   // 每次处理两字节
    {
        // 按 16 位分组，低位在左，高位在右
        sum += (*p << 8) + *(p + 1);
        p += 2;
    }
    if(len == 1)    // 奇数字节时
        sum += *(uint8_t *)p;   // 处理最后一个字节
    sum = (sum >> 16) + (sum & 0xffff); // 将产生的进位加到低16位
    sum += (sum >> 16); // 将上一行产生的进位再加
    result = ~sum;
    return result;
}

void pack_ipv4_hdr(ipv4_hdr_t *ipv4, uint32_t src, uint32_t dst, uint8_t protocol, uint16_t data_len)
{
    ipv4->version = 4;
    ipv4->ihl = sizeof(ipv4_hdr_t) / 4;
    ipv4->tos = 0;
    ipv4->tot_len = htons(sizeof(ipv4_hdr_t) + data_len);
    ipv4->id = htons(getpid());
    ipv4->frag = htons(DF);
    ipv4->ttl = 64;
    ipv4->protocol = protocol;
    ipv4->checksum = 0;
    ipv4->src_addr = (src);
    ipv4->dst_addr = (dst);
    ipv4->checksum = ipv4_checksum(ipv4, sizeof(ipv4_hdr_t));
}