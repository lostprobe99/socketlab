/**
 * common.h
 *
 * @author : lostprobe
 * @date   : 2025/05/04 13:43
 * @brief  : common utility for netpackets
 */

#ifndef NETPACKETS_COMMON_H
#define NETPACKETS_COMMON_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <errno.h>

#include <unistd.h>
#include <arpa/inet.h>
#include <net/if.h>

// #include <linux/if_ether.h>
#include <linux/if_arp.h>

#ifdef __cplusplus
extern "C" {
#endif

#define __packed __attribute__((packed))

#define ALL_MAC_BYTE(x) x[0], x[1], x[2], x[3], x[4], x[5]
#define ALL_IP_BYTE(x) x[0], x[1], x[2], x[3]

#define MAC_BYTE_LEN 6
#define MAC_STR_LEN 18

#define IPV4_BYTE_LEN 4
#define IPV4_STR_LEN 16

#define IPV6_BYTE_LEN 16
#define IPV6_STR_LEN 40

#define ETH_DATA_MIN_LEN 46
#define ETH_DATA_MAX_LEN 1500

#define ETH_FRAME_MIN_LEN (sizeof(ether_hdr_t) + ETH_DATA_MIN_LEN)
#define ETH_FRAME_MAX_LEN (sizeof(ether_hdr_t) + ETH_DATA_MAX_LEN)

#ifdef __cplusplus
}
#endif

#endif // COMMON_H