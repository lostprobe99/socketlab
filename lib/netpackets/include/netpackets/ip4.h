/**
 * ip4.h
 *
 * @author : lostprobe
 * @date   : 2025/05/04 14:48
 * @brief  : ip4.h
 */

#ifndef NETPACKETS_IP4_H
#define NETPACKETS_IP4_H

#include "common.h"

#ifdef __cplusplus
extern "C" {
#endif 

#define ETH_TYPE_IP4 0x0800

typedef struct __packed _ip4_hdr_ {
    struct {
        #if __BYTE_ORDER == __BIG_ENDIAN
        /* 版本 4bit */
        uint8_t version: 4;
        /*
         * 头部长度(internet header length, IHL), 
         * 表明有多少 4字节, 头部字节数 = IHL * 4字节, 
         * 如果不包含任何选项, 头部大小为 20 字节, ihl=5, 
         * ihl 的最大值为 15, 即 IP 头最大为 60 字节, 
         * */
        uint8_t ihl: 4;
        #elif  __BYTE_ORDER == __LITTLE_ENDIAN
        uint8_t ihl: 4;
        uint8_t version: 4;
        #else
        #error "unknown byte order"
        #endif
    };  // 1 byte
    union {
        struct {
        #if __BYTE_ORDER == __BIG_ENDIAN
            /* 区分服务(Differentiated Services, DS), 一般不使用 */
            uint8_t ds: 6;
            /* 显示拥塞通告(Explicit Congestion Notification, ECN) */
            uint8_t ecn: 2;
        #elif  __BYTE_ORDER == __LITTLE_ENDIAN
            uint8_t ecn: 2;
            uint8_t ds: 6;
        #else
        #error "unknown byte order"
        #endif
        };
        uint8_t tos;
    };  // 1 byte
    /*
     * IP包总长度(Total Length), 包含头部和数据, 单位是字节, 
     * 最大值 2^16 - 1 = 65535 字节,
     * 当 IP 包长度大于下层 MTU 时, 需要分片传输
     * */
    uint16_t tot_len;
    /* 标识符(Identification), 用于标识一个 IP 包的所有分片 */
    uint16_t id;
    union {
        #if 0
        struct {
            #if __BYTE_ORDER == __BIG_ENDIAN
            /*
            * 标识(Flags), 用于控制和识别分片的标志位, 
            * 第 0 位, 保留, 固定为 0,
            * 第 1 位, 禁止分片(Don't Fragment, DF), 值为 0 时才允许分片
            * 第 2 位, 更多分片(More Fragments, MF),
            *          值为 1 时表示后面还有分片,
            *          值为 0 时表示这是最后一个分片
            * */
            uint16_t flags: 3;
            /* 分片偏移(Fragment Offset), 表示一个分片相对于原始包开头的偏移量 */
            uint16_t frag_off: 13;
            #elif  __BYTE_ORDER == __LITTLE_ENDIAN
            uint16_t frag_off: 13;
            uint16_t flags: 3;
            #else
            #error "unknown byte order"
            #endif
        }; // 2 bytes
        #endif
        #define RF 0x8000   // reserved flag
        #define DF 0x4000   // dont fragment flag
        #define MF 0x2000   // more fragment flag
        uint16_t frag;
    };  // 2 bytes
    /*
     * 存活时间(Time To Live, TTL), 避免包陷入环路,
     * 每经过一个路由器减一, 达到 0 时包会被丢弃,
     * 一般设置为 64 或 128
     * */
    uint8_t ttl;
    /*
     * 协议(Protocol), 表示上层协议类型, 常见的有:
     * 1  ICMP 互联网控制消息协议(Internet Control Message Protocol)
     * 2  IGMP 互联网组管理协议(Internet Group Management Protocol), 就是组播的协议
     * 6  TCP  传输控制协议(Transmission Control Protocol)
     * 17 UDP  用户数据报协议(User Datagram Protocol)
     * */
    uint8_t protocol;
    /* 校验和(Checksum), 用于对 IP 头部查错, 不包含数据部分,
       每一跳都会重新计算校验和, 因为头部字段可能发生变化 */
    uint16_t checksum;
    /* 源 IP (Source Address) */
    uint32_t src_addr;
    /* 目的 IP (Destination Address) */
    uint32_t dst_addr;
    /* 可选字段, 选项 */
} ip4_hdr_t;    // 20 bytes

#ifdef __cplusplus
}
#endif

#endif // IP4_H