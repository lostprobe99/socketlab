/**
 * nl_get_ipaddr.c
 *
 * @author : lostprobe
 * @date   : 2025/04/26 17:24
 * @brief  : netlink hello world
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <linux/if_link.h>
#include <linux/netlink.h>
#include <linux/rtnetlink.h>
#include <net/if.h>
#include "simple_log.h"
#include "util.h"
#include <arpa/inet.h>

#define PAIR(x) (#x, x)
#define SPAIR(s, x) #x, (s).x
#define SPPAIR(s, x) #x, (s)->x
#define EPAIR(x) [x] = #x

#define dump_sockaddr_nl(nladdr) \
    log_debug("sockaddr_nl={%s=%d, %s=%d, %s=%d, %s=%d}\n" \
        , SPAIR(nladdr, nl_family)\
        , SPAIR(nladdr, nl_pid)   \
        , SPAIR(nladdr, nl_groups)\
        , SPAIR(nladdr, nl_pad)\
    )

#define dump_nlmsghdr(nlh) \
    log_debug("nlmsghdr={%s=%u, %s=%u, %s=%u, %s=%u, %s=%u}\n" \
        , SPAIR(nlh, nlmsg_len)\
        , SPAIR(nlh, nlmsg_type)   \
        , SPAIR(nlh, nlmsg_flags)\
        , SPAIR(nlh, nlmsg_seq)\
        , SPAIR(nlh, nlmsg_pid)\
    )

#define dump_ifaddrmsg(ifa) \
    log_debug("ifaddrmsg={%s=%u, %s=%u, %s=%u, %s=%u, %s=%u}\n" \
        , SPAIR(ifa, ifa_family)   /* 接口地址族 */ \
        , SPAIR(ifa, ifa_prefixlen) /* 地址的前缀长度 */   \
        , SPAIR(ifa, ifa_flags) /*  */ \
        , SPAIR(ifa, ifa_scope) /* 地址的作用域 */ \
        , SPAIR(ifa, ifa_index) /* 接口索引 */ \
    )

#define dump_rtattr(rta) \
    log_debug("rtattr={%s=%u, %s=%u}\n" \
        , SPAIR(rta, rta_len)\
        , SPAIR(rta, rta_type)   \
    )

const char * rta_type_str(uint16_t type)
{
    static const char * rta_type_strs[] = {
        EPAIR(IFA_UNSPEC),
        EPAIR(IFA_ADDRESS),
        EPAIR(IFA_LOCAL),
        EPAIR(IFA_LABEL),
        EPAIR(IFA_BROADCAST),
        EPAIR(IFA_ANYCAST),
        EPAIR(IFA_CACHEINFO),
        EPAIR(IFA_MULTICAST),
        EPAIR(IFA_FLAGS),
        EPAIR(IFA_RT_PRIORITY),  /* u32, priority/metric for prefix route */
        EPAIR(IFA_TARGET_NETNSID),
        EPAIR(__IFA_MAX),
    };
    if(type >= __IFA_MAX)
        return "";
    return rta_type_strs[type];
}

void init_simple_log()
{
    set_log_level(LOG_INFO);
    set_log_os_file("/tmp/nl_get_ipaddr.log");
}

int main(int argc, char ** argv)
{
    init_simple_log();
    // create netlink socket
    int sock = socket(AF_NETLINK, SOCK_RAW, NETLINK_ROUTE);
    if (sock < 0)
    {
        perror("socket");
        exit(EXIT_FAILURE);
    }
    log_info("create netlink socket: [%d]\n", sock);

    // struct sockaddr_nl nladdr;
    // memset(&nladdr, 0, sizeof(nladdr));
    // nladdr.nl_family = AF_NETLINK;
    // nladdr.nl_pid = getpid();
    // nladdr.nl_groups = 0;
    // nladdr.nl_pad = 0;
    // dump_sockaddr_nl(nladdr);

    // if (bind(sock, (const struct sockaddr *)&nladdr, sizeof(nladdr)) < 0)
    // {
    //     perror("bind");
    //     close(sock);
    //     exit(EXIT_FAILURE);
    // }

    struct {
        struct nlmsghdr nlh;    // netlink 消息头
        struct ifaddrmsg ifa;   // 接口地址信息
    } req;
    memset(&req, 0, sizeof(req));
    req.nlh.nlmsg_len = NLMSG_LENGTH(sizeof(req));  // 设置消息长度
    req.nlh.nlmsg_type = RTM_GETADDR;   // 消息类型为 RTM_GETADDR，获取地址信息
    req.nlh.nlmsg_flags = NLM_F_DUMP | NLM_F_REQUEST;   // 标志位
    req.nlh.nlmsg_seq = 0;
    req.nlh.nlmsg_pid = getpid();
    dump_nlmsghdr(req.nlh);

    req.ifa.ifa_family = AF_UNSPEC; // 地址族为 AF_UNSPEC，表示所有地址族
    req.ifa.ifa_scope = RT_SCOPE_UNIVERSE;
    dump_ifaddrmsg(req.ifa);

    log_info("send netlink message\n");
    // __addr = NULL 表示发送给内核
    if(sendto(sock, &req, req.nlh.nlmsg_len, 0, NULL, 0) < 0)
    {
        perror("send");
        close(sock);
        exit(EXIT_FAILURE);
    }

    // netlink 返回的消息
    // +-----------+
    // | nlmsghdr  |
    // | ifaddrmsg |
    // | rtattr    |
    // | ...       |
    // | rtattr    |
    // +-----------+
    // | nlmsghdr  |
    // | ifaddrmsg |
    // | rtattr    |
    // | ...       |
    // | rtattr    |
    // +-----------+
    // 构造 msghdr 用于接收数据
    char buf[8192];
    memset(buf, 0, sizeof(buf));
    struct iovec iov = {
        .iov_base = buf,
        .iov_len = sizeof(buf)
    };
    struct msghdr msg = {
        .msg_iov = &iov,
        .msg_iovlen = 1,
    };

    ssize_t n = 0;
    struct nlmsghdr *nh;
    struct ifaddrmsg *ifa;
    char str[256] = {0};
try_recv:
    n = recvmsg(sock, &msg, 0);
    if (n < 0) {
        perror("recvmsg");
        close(sock);
        exit(EXIT_FAILURE);
    }
    log_info("recvmsg: %zd bytes\n", n);
    log_debug_hexdump("", (uint8_t *)buf, n);

    nh = (struct nlmsghdr *)buf;
    while(nh->nlmsg_type  != NLMSG_DONE)
    {
        for (; NLMSG_OK(nh, n); nh = NLMSG_NEXT(nh, n)) {
            dump_nlmsghdr(*nh);
            if(nh->nlmsg_type == NLMSG_DONE) {
                // NLMSG_DONE 表示消息结束
                log_info("NLMSG_DONE\n");
                break;
            }
            if(nh->nlmsg_type == NLMSG_ERROR) {
                // NLMSG_ERROR 表示错误
                log_info("NLMSG_ERROR\n");
                struct nlmsgerr *err = NLMSG_DATA(nh);
                // err->error = 0 表示没有错误
                log_info("NLMSG_ERROR: error %d\n", err->error);
                break;
            }
            ifa = NLMSG_DATA(nh);
            dump_ifaddrmsg(*ifa);

            struct rtattr *rta = IFA_RTA(ifa);
            int len = IFA_PAYLOAD(nh);

            while (RTA_OK(rta, len)) {
                dump_rtattr(*rta);
                if(rta->rta_type == IFA_ADDRESS || rta->rta_type == IFA_BROADCAST)
                {
                    log_info("dev name: %s\n", if_indextoname(ifa->ifa_index, str));
                    log_info("%s: %s\n", rta_type_str(rta->rta_type), inet_ntop(ifa->ifa_family, RTA_DATA(rta), str, sizeof(str)));
                }
                rta = RTA_NEXT(rta, len);
            }
        }
        goto try_recv;
    }

    return 0;
}