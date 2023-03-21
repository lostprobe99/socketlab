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

#define MAX_EVENT 1024
#define BUF_SIZE 1024

typedef struct epoll_event epoll_event_t;

int main(int argc, char ** argv)
{
    int sockfd = server(9000);
    Assert(sockfd == -1, "server() error");
    int epfd = epoll_create1(0);
    Assert(epfd == -1, "epoll_create1() error");
    epoll_event_t events[MAX_EVENT], ev;
    ev.events = EPOLLIN;    // ET 模式，未处理错误
    ev.data.fd = sockfd;
    sockaddr_in clnt_addr;
    socklen_t clnt_addr_len;
    epoll_ctl(epfd, EPOLL_CTL_ADD, sockfd, &ev);    // 将服务器 socket fd 添加到 epoll

    while(1) // 监听 epoll 上的事件
    {
        int nfds = epoll_wait(epfd, events, MAX_EVENT, -1); // 有 nfds 个 fd 发生事件
        for(int i = 0; i < nfds; ++i)
        {
            if(events[i].data.fd == sockfd) // 当前服务器 fd 发生事件，表明有新的客户端连接
            {
                int clnt_fd = accept(sockfd, (sockaddr*)&clnt_addr, &clnt_addr_len);
                ev.data.fd = clnt_fd;
                ev.events = EPOLLIN | EPOLLET;  // ET 模式处理客户端连接
                fcntl(clnt_fd, F_SETFL, fcntl(clnt_fd, F_GETFL) | O_NONBLOCK);    // ET 需配合非阻塞 socket 使用
                epoll_ctl(epfd, EPOLL_CTL_ADD, clnt_fd, &ev);   // 将客户端 fd 添加到 epoll
            }
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
        }
    }

    return 0;
}