/**
 * packet_buff.c
 *
 * @author : lostprobe
 * @date   : 2026/02/20 16:37
 * @brief  : packet_buff.c
 */
#include "netpackets/packet_buff.h"

struct packet_buff * pktb_alloc(size_t size)
{
    struct packet_buff *pktb = NULL;
    pktb = malloc(sizeof(struct packet_buff) + size);
    if (!pktb)
        return NULL;
    memset(pktb, 0, sizeof(struct packet_buff) + size);
    pktb->size = sizeof(struct packet_buff) + size;
    pktb->data = (uint8_t *)pktb + sizeof(struct packet_buff);
    pktb->capacity = size;
    pktb->data_len = 0;
    pktb->head = pktb->data;
    pktb->end = pktb->data + size;
    pktb->tail = pktb->head;
    return pktb;
}

void pktb_free(struct packet_buff *pktb)
{
    free(pktb);
}

void pktb_reset(struct packet_buff *pktb)
{
    pktb->tail = pktb->head;
    pktb->data_len = 0;
}

void* pktb_push(struct packet_buff *pktb, size_t len)
{
    uint8_t *p = pktb->tail;
    if (pktb->end - pktb->tail < len)
        return NULL;
    pktb->tail += len;
    pktb->data_len += len;
    return p;
}

void* pktb_pop(struct packet_buff *pktb, size_t len)
{
    if (pktb->tail - pktb->head < len)
        return NULL;
    pktb->tail -= len;
    pktb->data_len -= len;
    return pktb->tail;
}

uint32_t pktb_size(struct packet_buff *pktb)
{
    return pktb->size;
}

uint32_t pktb_data_len(struct packet_buff *pktb)
{
    return pktb->data_len;
}