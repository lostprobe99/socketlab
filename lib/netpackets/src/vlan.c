/**
 * vlan.c
 *
 * @author : lostprobe
 * @date   : 2026/02/20 14:00
 * @brief  : vlan.c
 */
#include "netpackets/vlan.h"

void pack_vlan_hdr(vlan_hdr_t *vlan_hdr, uint16_t id, uint16_t dei, uint16_t pri, uint16_t proto_type)
{
    vlan_hdr->id = id;
    vlan_hdr->dei = dei;
    vlan_hdr->pri = pri;
    vlan_hdr->tci = htons(vlan_hdr->tci);
    vlan_hdr->proto_type = htons(proto_type);
}