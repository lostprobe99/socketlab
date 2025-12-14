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
#include <math.h>
#include <sys/poll.h>

#include "netpackets/sock_if.h"
#include "netpackets/icmp.h"
#include "netpackets/ip4.h"

#include "simple_log.h"

#include "debug.h"
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
    uint64_t timestamp;  // 接收时间，单位微秒
    struct sockaddr_in addr; // 发送方地址
    ipv4_hdr_t ipv4_hdr;   // ip报文头
    ping_packet_t icmp; // icmp报文
}recv_data_t;

struct ping_stats {
    uint64_t start; // timestamp for the program start
    uint64_t sent;  // tx
    uint64_t recv;  // rx
    uint64_t lost;  // loss
    uint64_t min_rtt;
    uint64_t max_rtt;
    uint64_t avg_rtt;
    /*
     * 样本标准差
     * 无论发10 个还是 100 个 ping 包，都只是对网络状态的一次抽样
     * ping 判断的是“这条路径 RTT 的真实波动”，而不是“这几次 ping 的波动”
     * Linux 原生 ping 命令输出的 mdev 也是 样本标准差
    */
    double mdev_rtt;  // 样本标准差
    uint64_t total_rtt;
    struct {
        uint64_t c; // count for rtt
        double mean;  // avg
        double m2;    // sum of squares of differences from mean // 平方偏差和
        // mdev^2 = m2 / c
    } rtt_stats;    // for mdev rtt
    #define mean g_stats.rtt_stats.mean
    #define m2 g_stats.rtt_stats.m2
    #define c g_stats.rtt_stats.c
} g_stats = {0};

void usage(char *prog)
{
    printf("Usage: %s <ip> [count]\n", prog);
}

// Welford's method
void update_rtt_stats(uint64_t rtt)
{
    // mean_{n+1} = mean_n + (x - mean_n) / (n + 1)
    // m2_{n+1} = m2_n + (x - mean_n) * (x - mean_{n+1})

    // d1 = x - mean_n, d2 = x - mean_{n+1}

    // mean_{n+1} = mean_n + d / (n + 1)
    // m2_{n+1} = m2_n + d * (x - mean_{n+1})
    double d1, d2;  // 后面立即赋值，所以不进行初始化
    double x = (double)rtt; // 计算中全部使用 double

    // 1. 计算 d1
    d1 = x - mean;    // 计算 x - mean_n
    c++;    // 计数 n + 1
    // 2. 更新平均值 mean
    mean = mean + (1.0 * d1 / c);  // mean_{n+1}

    // 3. 计算 d2
    d2 = x - mean;    // 此时 mean = mean_{n+1}
    // 4. 计算 m2
    m2 = m2 + d1 * d2;
}

// 统计 rtt (max/min/avg/mdev) 其中 rtt 的单位是微秒
void rtt_stats(uint64_t rtt, struct ping_stats *pstats)
{
    static int rtt_count = 0;
    rtt_count++;
    if(pstats->min_rtt == 0)
        pstats->min_rtt = rtt;
    pstats->total_rtt += rtt;
    pstats->min_rtt = u64_min(pstats->min_rtt, rtt);
    pstats->max_rtt = u64_max(pstats->max_rtt, rtt);
    update_rtt_stats(rtt);
}

void print_stats(int argc, void *ip)
{
    uint64_t diff = get_timestamp_us() - g_stats.start;

    // 若没有发包，则退出
    if(g_stats.sent == 0)
        return;

    // g_stats.avg_rtt = g_stats.total_rtt / g_stats.recv;
    g_stats.avg_rtt = mean;

    g_stats.mdev_rtt = sqrt(m2 / (c - 1));

    printf("--- %s ping statistics ---\n", (char *)ip);
    printf("tx=%lu, rx=%lu, loss=%lu (%.1lf%%), ping use time %lu ms\n",
        g_stats.sent,
        g_stats.recv,
        g_stats.lost,
        1.0 * g_stats.lost / g_stats.sent,
        diff / 1000u
    );
    printf("rtt min/avg/max/mdev = %.3f/%.3f/%.3f/%.3f ms\n",
        g_stats.min_rtt / 1000.0, g_stats.avg_rtt / 1000.0, g_stats.max_rtt / 1000.0, g_stats.mdev_rtt / 1000.0);
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

int recv_icmp_echo(int sock, recv_data_t* d)
{
    uint8_t buf[512];
    sockaddr_in *from = &d->addr;
    socklen_t from_len = sizeof(sockaddr_in);
    ping_packet_t *recv_icmp = &(d->icmp);
    ipv4_hdr_t *ipv4_hdr = &(d->ipv4_hdr);
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

    n = recvfrom(sock, buf, sizeof(buf), 0, (sockaddr*)from, &from_len);
    d->timestamp = get_timestamp_us();
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
    memcpy(ipv4_hdr, buf, sizeof(ipv4_hdr_t));
    ipv4_ntohs(ipv4_hdr);
    ihl = ipv4_hdr->ihl;    // 提取 ihl
    ihl *= 4;   // 计算 ipv4 头长度
    // buf + ihl = skip ipv4 header
    memcpy(recv_icmp, buf + ihl, sizeof(ping_packet_t));
    recv_icmp->echo.identifier = ntohs(recv_icmp->echo.identifier);
    recv_icmp->echo.sequence = ntohs(recv_icmp->echo.sequence);
    recv_icmp->hdr.checksum = ntohs(recv_icmp->hdr.checksum);
    strncpy(s, recv_icmp->data, sizeof(recv_icmp->data));
    log_debug("接收到的ICMP数据包：\n"
              "\ttype: %d\n"
              "\tcode: %d\n"
              "\tchecksum: %d\n"
              "\tidentifier: %d\n"
              "\tsequence: %d\n"
              "\tdata: %s\n",
              recv_icmp->hdr.type,
              recv_icmp->hdr.code,
              recv_icmp->hdr.checksum,
              recv_icmp->echo.identifier,
              recv_icmp->echo.sequence,
              s
    );

    return RECV_SUCCESS;
}

int ping(const char *ip, int n)
{
    uint64_t start = 0, end = 0, rtt = 0;
    recv_data_t recv_data;
    ping_packet_t *icmp = &recv_data.icmp;
    ipv4_hdr_t *ipv4_hdr = &recv_data.ipv4_hdr;
    int recv_ret = RECV_SUCCESS;
    int sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
    if( sockfd < 0 )
    {
        // 即使 log level 为 off 也可以执行到 die
        printf("socket() error: %s\n", strerror(errno));
        exit(1);
    }
    printf("PING %s ...\n", ip);

    for(int i = 0; i < n; i++)
    {
        start = get_timestamp_us();
        log_info("发送时间：%lu us\n", start);
        send_icmp_echo(sockfd, ip, i + 1);
        g_stats.sent++;

        recv_ret = recv_icmp_echo(sockfd, &recv_data);
        end = recv_data.timestamp;
        rtt = end - start;
        // 检查接收结果
        if(recv_ret == RECV_SUCCESS)
        {
            g_stats.recv++;
            log_info("接收时间：%lu, 用时：%.3lf ms\n", end, rtt / 1000.0);
            // 检查 ICMP 响应包
            if(icmp->hdr.type == ICMP_TYPE_ECHO_REPLY && icmp->hdr.code == 0)
            {
                rtt_stats(rtt, &g_stats);
                printf("%d bytes from %s: seq=%d, ttl=%d, rtt=%.3lf ms\n",
                    MAGIC_LEN,
                    inet_ntoa(recv_data.addr.sin_addr),
                    i + 1,
                    ipv4_hdr->ttl,
                    rtt / 1000.0
                );
            }
            else
            {
                g_stats.lost++;
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
    g_stats.start = get_timestamp_us();
    simple_log_set_log_os(stdout);
    set_log_level(LOG_LEVEL_OFF);
    
    on_exit(print_stats, argv[1]);
    if(argc < 2)
    {
        usage(argv[0]);
        return -1;
    }

    ping(argv[1], argv[2] == NULL ? 4 : atoi(argv[2]));

    return 0;
}