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

#include "netpackets/sock_if.h"
#include "netpackets/icmp.h"

#include "simple_log.h"

#include "common.h"
#include "util.h"

#define ICMP_TYPE_ECHO_REQUEST 8
#define ICMP_TYPE_ECHO_REPLY 0

#define MAGIC_STR "0123456789abcdefAB"
#define MAGIC_LEN 18

typedef struct __packed _ping_packet_ {
    icmp_hdr_t hdr;
    icmp_echo_t echo;
    char data[MAGIC_LEN];
} ping_packet_t;

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

    n = recvfrom(sock, buf, sizeof(buf), 0, &from, &from_len);
    // 接收响应包
    if(n < 0)
    {
        if(errno == EAGAIN || errno == EWOULDBLOCK)
            return 0;
        else
            return -1;
    }
    log_debug_hexdump("recv buf", buf, n);
    log_debug("接收数据包长度：%d\n", n);
    log_debug_hexdump("recv buf", buf, n);
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
    int sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
    if( sockfd < 0 )
    {
        log_die("socket() error: %m\n");
    }
    log_info("ping %s\n", ip);

    for(int i = 0; i < n; i++)
    {
        start = timestamp();
        log_info("发送时间：%lu\n", start);
        send_icmp_echo(sockfd, ip, i + 1);

        recv_icmp_echo(sockfd, &recv_pack);
        end = timestamp();
        log_info("接收时间：%lu, 用时：%lu ms\n", end, end - start);
        if(recv_pack.hdr.type == ICMP_TYPE_ECHO_REPLY && recv_pack.hdr.code == 0)
        {
            printf("%s seq: %d, time: %lu ms\n", ip, i + 1, end - start);
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