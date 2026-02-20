/**
 * packet_buff.h
 *
 * @author : lostprobe
 * @date   : 2026/02/20 16:36
 * @brief  : packet_buff.h
 */
#ifndef PACKET_BUFF_H
#define PACKET_BUFF_H

#ifdef __cplusplus
extern "C" {
#endif

#include "common.h"

struct packet_buff {
    uint8_t *data;  // data 区首地址
    uint8_t *head;  // 固定指向 data 区首地址
    uint8_t *end;   // 固定指向 data 区末尾
    uint8_t *tail;  // 指向当前位置
    uint32_t data_len;  // data 区已使用的长度
    uint32_t capacity;  // data 区总容量
    uint32_t size;  // 整个 packet_buff 的大小
};

/// @brief 创建一个 packet_buff
/// @param size data 区的大小
/// @return packet_buff 指针
struct packet_buff * pktb_alloc(size_t size);

/// @brief 释放 packet_buff
void pktb_free(struct packet_buff *pktb);

/// @brief 重置 packet_buff 中指针的指向
void pktb_reset(struct packet_buff *pktb);

/// @brief 在 pktb 中分配一段长度为 len 的空间, 返回首地址
void* pktb_push(struct packet_buff *pktb, size_t len);

/// @brief 从 pktb 中弹出 len 长度的空间, 返回首地址
void* pktb_pop(struct packet_buff *pktb, size_t len);

/// @brief 获取 packet_buff 的大小
uint32_t pktb_size(struct packet_buff *pktb);

/// @brief 获取 packet_buff 的 data 区已使用的长度
uint32_t pktb_data_len(struct packet_buff *pktb);

#ifdef __cplusplus
}
#endif

#endif // PACKET_BUFF_H