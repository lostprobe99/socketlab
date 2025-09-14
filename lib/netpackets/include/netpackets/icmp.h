/**
 * icmp.h
 *
 * @author : 通义灵码
 * @modified: lostprobe
 * @date   : 2025/05/24 13:35
 * @brief  : icmp.h
 */
#ifndef NETPACKETS_ICMP_H
#define NETPACKETS_ICMP_H

#ifdef __cplusplus
extern "C" {
#endif

#include "common.h"

/**
 * @brief 计算 ICMP 校验和
 *
 * @param data 指向要计算校验和的数据缓冲区的指针
 * @param len  数据缓冲区的长度（以字节为单位）
 *
 * @return 返回计算得到的 16 位校验和值
 */
uint16_t icmp_checksum(void *data, size_t len);

// ICMP header structure
typedef struct __packed _icmp_hdr_ {
    /*
     * ICMP 报文类型，用于标识错误类型的差错报文或查询类型的报告报文。
     * 常用类型：type = 0, code = 0: 表示回显应答(ping 应答)
     *          type = 8, code = 0: 表示回显请求(ping 请求)
     *          type = 3, code = 0: 网络不可达
     *          type = 11, code = 0: 超时
     *          type = 5, code = 0: 重定向
     * */
    uint8_t type;     // 消息类型
    /* ICMP 差错报文的类型，进一步表示错误原因 */
    uint8_t code;     // 子类型代码
    /* ICMP 消息的校验和, 校验方法和 IP 头的校验和一致 */
    uint16_t checksum;
} icmp_hdr_t;

// -- ICMP Echo(ping) 请求/响应 (type 8 / 0)
typedef struct __packed _icmp_echo_ {
    /* 标识符, 对每一个报文进行标识 */
    uint16_t identifier;
    /* 序列号, 对每一个报文进行编号, 比如：第一个序号为 1, 第二个为 2 */
    uint16_t sequence;
} icmp_echo_t;

#if 0
// -- ICMP 目标不可达消息 (type 3)
typedef struct __packed _icmp_unreach_ {

};
#endif

#ifdef __cplusplus
}
#endif

#endif