/**
 * 8021q.h
 *
 * @author : lostprobe
 * @date   : 2025/05/08 20:55
 * @brief  : 8021q.h
 */
#ifndef NETPACKETS_8021Q_H
#define NETPACKETS_8021Q_H

#include "common.h"

#ifdef __cplusplus
extern "C" {
#endif

#define ETH_TYPE_8021Q 0x8100

typedef struct __packed _vlan_hdr_ {
    union {
        /* Tag Control Information, TCI */
        uint16_t tci;
        /* include below */
        struct {
            #if __BYTE_ORDER == __BIG_ENDIAN
            /* VLAN Priority, 优先级 */
            uint16_t pri: 3;
            /* Drop Eligible Indicator,DEI 表示该帧是否可丢弃 */
            uint16_t dei: 1;
            /* VLAN ID, 802.1q VLAN 标识符 */
            uint16_t id: 12;
            #elif __BYTE_ORDER == __LITTLE_ENDIAN
            uint16_t id: 12;
            uint16_t dei: 1;
            uint16_t pri: 3;
            #elif
            #error "unknown byte order"
            #endif
        };
    };
    /* 上层协议类型 */
    uint16_t proto_type;
} vlan_hdr_t;

void pack_vlan_hdr(vlan_hdr_t *vlan_hdr, uint16_t id, uint16_t dei, uint16_t pri, uint16_t proto_type);

#ifdef __cplusplus
}
#endif

#endif // 8021Q_H