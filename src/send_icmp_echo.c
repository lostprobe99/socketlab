/**
 * send_icmp_echo.c
 *
 * @author : lostprobe
 * @date   : 2025/09/13 17:09
 * @brief  : send_icmp_echo.c
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/poll.h>

#include "netpackets/sock_if.h"
#include "netpackets/icmp.h"
#include "netpackets/ip4.h"

#include "simple_log.h"

#include "common.h"
#include "util.h"

#define ICMP_TYPE_ECHO_REQUEST 8
#define ICMP_TYPE_ECHO_REPLY 0

#define MAGIC_STR "0123456789abcdefAB"
#define MAGIC_LEN 18

/*
type=3, code=0, 网络不可达, 路由表中无路径
type=3, code=1, 主机不可达, 路由器回复主机不可达
type=3, code=2, 协议不可达, IP 层已将数据包送达, 但 IP 头中的 协议(protocol) 字段不支持
type=3, code=3, 端口不可达
type=3, code=4, 需要分片
type=11,code=0, TTL超时, 在传输过程中, ttl 减为 0
**/

enum error_code{
    RECV_SUCCESS = 0,
    RECV_TIMEOUT = -1,
    RECV_ERROR = -2,
    RECV_BAD_PACKET = -3,
};

typedef struct __packed _ping_packet_ {
    icmp_hdr_t hdr;
    icmp_echo_t echo;
    char data[MAGIC_LEN];
} ping_packet_t;

typedef struct recv_packet {
    uint64_t time_recv;  // 接收时间
    struct sockaddr_in addr; // 发送方地址
    ip4_hdr_t ip;   // ip报文头
    ping_packet_t icmp; // icmp报文
}recv_data_t;

void usage(char *prog)
{
    printf("Usage: %s <ip> [count]\n", prog);
}

// 1071 checksum
uint16_t icmp_checksum(void *data, size_t len)
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

int send_icmp_echo(int sock, const char *ip, int seq)
{ 
    ping_packet_t packet;
    uint16_t checksum1 = 0;
    sockaddr_in addr = make_sockaddr(AF_INET, ip, 0);

    // 接收方地址
    log_debug_hexdump("ping destination", &addr, sizeof(addr));

    // 组装 ping 请求包
    packet.hdr.type = ICMP_TYPE_ECHO_REQUEST;
    packet.hdr.code = 0;
    packet.hdr.checksum = 0;
    packet.echo.identifier = htons(getpid() & 0xffff);
    packet.echo.sequence = htons(seq);
    memcpy(packet.data, MAGIC_STR, MAGIC_LEN);
    checksum1 = icmp_checksum(&packet, sizeof(packet));
    packet.hdr.checksum = htons(checksum1);
    log_debug("checksum1: %x, packet.checksum: %x\n", checksum1, packet.hdr.checksum);
    log_debug_hexdump("ping request", &packet, sizeof(packet));

    int n = sendto(sock, &packet, sizeof(packet), 0, (sockaddr *)&addr, sizeof(addr));
    return n;
}

int recv_icmp_echo(int sock, ping_packet_t *recv_pack)
{
    uint8_t buf[512];
    sockaddr from;
    socklen_t from_len = sizeof(from);
    // ping_packet_t recv_pack = {0};
    char s[MAGIC_LEN + 1] = {0};
    int n = 0, ihl = 0;
    struct pollfd pfd = {
        .fd = sock,
        .events = POLLIN,
    };

    // 使用 poll 实现等待超时
    int ret = poll(&pfd, 1, 1000);
    if ( ret == -1 )
    {
        log_error("poll(): %m\n");
        return RECV_ERROR;
    }
    if ( ret == 0 )
    {
        log_error("recv timeout\n");
        return RECV_TIMEOUT;
    }

    n = recvfrom(sock, buf, sizeof(buf), 0, &from, &from_len);
    // 接收响应包
    if(n < 0)
    {
        if(errno == EAGAIN || errno == EWOULDBLOCK)
            return RECV_TIMEOUT;
        else
        {
            log_error("recvfrom(): %m\n");
            return RECV_ERROR;
        }
    }

    log_debug_hexdump("recv buf", buf, n);
    log_debug("接收数据包长度：%d\n", n);

    if(n < sizeof(icmp_hdr_t) + sizeof(icmp_echo_t))
    {
        // 不完整的 ICMP 包
        log_error("incomplete ICMP packet\n");
        return RECV_BAD_PACKET;
    }
    ihl = buf[0] & 0x0f;    // 提取 ihl
    ihl *= 4;   // 计算 ipv4 头长度
    // buf + ihl skip ipv4 header
    memcpy(recv_pack, buf + ihl, sizeof(ping_packet_t));
    recv_pack->echo.identifier = ntohs(recv_pack->echo.identifier);
    recv_pack->echo.sequence = ntohs(recv_pack->echo.sequence);
    recv_pack->hdr.checksum = ntohs(recv_pack->hdr.checksum);
    strncpy(s, recv_pack->data, sizeof(recv_pack->data));
    log_debug("接收到的ICMP数据包：\n"
              "\ttype: %d\n"
              "\tcode: %d\n"
              "\tchecksum: %d\n"
              "\tidentifier: %d\n"
              "\tsequence: %d\n"
              "\tdata: %s\n",
              recv_pack->hdr.type,
              recv_pack->hdr.code,
              recv_pack->hdr.checksum,
              recv_pack->echo.identifier,
              recv_pack->echo.sequence,
              s
    );

    return 0;
}

int ping(const char *ip, int n)
{
    uint64_t start = 0, end = 0;
    ping_packet_t recv_pack;
    int recv_ret = RECV_SUCCESS;
    int sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
    if( sockfd < 0 )
    {
        // 即使 log level 为 off 也可以执行到 die
        log_die("socket() error: %m\n");
    }
    printf("ping %s\n", ip);

    for(int i = 0; i < n; i++)
    {
        start = timestamp();
        log_info("发送时间：%lu\n", start);
        send_icmp_echo(sockfd, ip, i + 1);

        recv_ret = recv_icmp_echo(sockfd, &recv_pack);
        end = timestamp();
        // 检查接收结果
        if(recv_ret == RECV_SUCCESS)
        {
            log_info("接收时间：%lu, 用时：%lu ms\n", end, end - start);
            // 检查 ICMP 响应包
            if(recv_pack.hdr.type == ICMP_TYPE_ECHO_REPLY && recv_pack.hdr.code == 0)
            {
                printf("from %s: seq=%d, rtt=%lu ms\n", ip, i + 1, end - start);
            }
        }
        else if(recv_ret == RECV_TIMEOUT)
        {
            printf("recv timeout, use: %lu ms\n", end - start);
        }
        else if(recv_ret == RECV_ERROR)
        {
            printf("recv error\n");
        }
        else if(recv_ret == RECV_BAD_PACKET)
        {
            printf("recv bad packet\n");
        }
    }

    return 0;
}

int main(int argc, char ** argv)
{
    simple_log_set_log_os(stdout);
    set_log_level(LOG_LEVEL_OFF);

    if(argc < 2)
    {
        usage(argv[0]);
        return -1;
    }

    ping(argv[1], argv[2] == NULL ? 4 : atoi(argv[2]));

    return 0;
}