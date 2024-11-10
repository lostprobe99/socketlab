/**
 * epoll.c
 *
 * @author : lostprobe
 * @date   : 2023/03/09 23:02
 * @brief  : epoll.c
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include <fcntl.h>
#include <sys/epoll.h>

#include "common.h"
#include "debug.h"
#include "buffer.h"

#define MAX_EVENT 1024
#define BUF_SIZE 1024

typedef struct epoll_event epoll_event_t;

int main(int argc, char ** argv)
{
    if(argc != 2)
    {
        printf("Usage: %s <port>\n", argv[0]);
        exit(1);
    }
    int port = atoi(argv[1]);
    errno = 0;
    int sockfd = server(port), clnt_fd;
    Assert(sockfd != -1, "server() error: %s", strerror(errno));
    NORM("Server starting at %d...", port);
    // select 需要在程序中定义 fd_set, epoll 下为操作系统保存
    int epfd = epoll_create1(0);
    Assert(epfd != -1, "epoll_create1() error: %s", strerror(errno));
    // epoll_event 用于保存 fd
    epoll_event_t events[MAX_EVENT], ev;
    ev.events = EPOLLIN;    // 需要读数据的情况
    ev.data.fd = sockfd;
    sockaddr_in clnt_addr;
    socklen_t clnt_addr_len = sizeof(clnt_addr);
    // 添加和删除监视对象, select 下有FD_SET等函数实现, epoll 下通过 epoll_ctl 请求操作系统执行
    epoll_ctl(epfd, EPOLL_CTL_ADD, sockfd, &ev);    // 将服务器 socket fd 添加到 epoll

    char buf[64] = {0};
    int buf_len = 0;
    int buf_size = sizeof(buf);
    while(1) // 监听 epoll 上的事件
    {
        // epoll_wait 等待 fd 发生变化
        // 对应于 select 函数
        int nfds = epoll_wait(epfd, events, MAX_EVENT, -1); // 有 nfds 个 fd 发生事件
        if(nfds == -1)
        {
            ERROR("epoll_wait() error: %s", strerror(errno));
            break;
        }
        printf(EVAL(nfds, "%d"));
        for(int i = 0; i < nfds; ++i)
        {
            printf(EVAL(events[i].data.fd, "%d"));
            if(events[i].data.fd == sockfd) // 当前服务器 fd 发生事件，表明有新的客户端连接
            {
                clnt_fd = accept(sockfd, (sockaddr*)&clnt_addr, &clnt_addr_len);
                ev.data.fd = clnt_fd;
                ev.events = EPOLLIN;  // | EPOLLET;  // ET 模式处理需要读数据的情况
                // fcntl(clnt_fd, F_SETFL, fcntl(clnt_fd, F_GETFL) | O_NONBLOCK);    // ET 需配合非阻塞 socket 使用
                epoll_ctl(epfd, EPOLL_CTL_ADD, clnt_fd, &ev);   // 将客户端 fd 添加到 epoll
                printf("Got client: fd = %d, addr = %s\n", clnt_fd, inet_ntoa(clnt_addr.sin_addr));
            }
            else
            {
                INFO("reading...");
                buf_len = read(events[i].data.fd, buf, buf_size);
                buf[buf_len] = 0;
                INFO("reading end.");
                if(buf_len == 0)   // 连接关闭
                {
                    // 将该 fd 移除监视
                    epoll_ctl(epfd, EPOLL_CTL_DEL, events[i].data.fd, NULL);
                    close(events[i].data.fd);
                    printf("Client = {fd = %d} closed\n", events[i].data.fd);
                }
                else
                {
                    printf("Got msg = {`%s`} from client = {fd = %d}\n", buf, events[i].data.fd);
                    write(events[i].data.fd, buf, buf_len);
                }
            }
            #if 0
            else if(events[i].events & EPOLLIN)
            {
                // handle event
                char buf[BUF_SIZE];
                ssize_t bytes_read = read(events[i].data.fd, buf, sizeof(buf));
                if(bytes_read > 0)
                {

                }
                else if(bytes_read == -1 && errno == EINTR)
                {
                    // 客户端正常中断
                    continue;
                }
                //                                          非阻塞 IO，表示数据读完
                else if(bytes_read == -1 && errno == EAGAIN || errno == EWOULDBLOCK)
                {
                    break;
                }
                else if(bytes_read == 0)
                {
                    // EOF，表示客户端断开连接
                    close(events[i].data.fd);   // 关闭 socket 会自动将 fd 从 epoll 中移除
                    break;
                }
                // 其他错误
            }
            #endif
        }
    }
    close(sockfd);
    close(epfd);

    return 0;
}