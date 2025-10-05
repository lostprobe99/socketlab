/**
 * ipv4.c
 *
 * @author : lostprobe
 * @date   : 2025/10/05 12:42
 * @brief  : ipv4.c
 */

#include "netpackets/ip4.h"

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