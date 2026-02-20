/**
 * send_vlan.c
 *
 * @author : lostprobe
 * @date   : 2026/02/20 15:26
 * @brief  : send_vlan.c
 */

#include <stdio.h>
#include <stdlib.h>

#include <netpackets/ether.h>
#include <netpackets/vlan.h>
#include <netpackets/ipv4.h>
#include <netpackets/common.h>
#include <netpackets/sock_if.h>
#include <netpackets/packet_buff.h>

#include <simple_log.h>

#define dump_pktb(pktb) \
    do { \
        log_debug("struct packet_buff {"); \
        printf("data = %p", pktb->data); \
        printf(", head = %p", pktb->head); \
        printf(", end = %p", pktb->end); \
        printf(", tail = %p", pktb->tail); \
        printf(", data_len = %d", pktb->data_len); \
        printf(", capacity = %d", pktb->capacity); \
        printf(", size = %d", pktb->size); \
        printf("}\n"); \
    } while (0)


int main()
{
    set_log_level(LOG_LEVEL_DEBUG);
    simple_log_set_log_os(stdout);
    uint8_t smac[] = { 0x00, 0x11, 0x22, 0x33, 0x44, 0x55 };
    uint8_t dmac[] = { 0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff };
    uint32_t sip = inet_addr("192.168.5.2");
    uint32_t dip = inet_addr("192.168.5.3");
    char data[] = "hello world";
    struct packet_buff *pktb = pktb_alloc(
        sizeof(ether_hdr_t) + 
        sizeof(vlan_hdr_t) + 
        sizeof(ipv4_hdr_t) + 
        sizeof(data)
    );

    ether_hdr_t *eth = pktb_push(pktb, sizeof(ether_hdr_t));
    vlan_hdr_t *vlan = pktb_push(pktb, sizeof(vlan_hdr_t));
    ipv4_hdr_t *ipv4 = pktb_push(pktb, sizeof(ipv4_hdr_t));
    char *p = pktb_push(pktb, sizeof(data));

    pack_ether_hdr(eth, smac, dmac, ETH_TYPE_8021Q);
    pack_vlan_hdr(vlan, 100, 0, 7, ETH_TYPE_IPV4);
    pack_ipv4_hdr(ipv4, sip, dip, 40, sizeof(data));
    memcpy(p, data, sizeof(data));

    dump_pktb(pktb);
    log_debug_hexdump("pktb", pktb, pktb_size(pktb));

    if(send_frame("eth0", pktb->data, pktb_data_len(pktb)) < 0)
    {
        log_error("send frame failed: %m\n");
    } else {
        log_info("send frame success\n");
    }

    pktb_free(pktb);

    return 0;
}