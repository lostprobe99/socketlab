/**
 * mp.c
 *
 * @author : lostprobe
 * @date   : 2023/02/11 11:19
 * @brief  : mp.c
 */

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/tcp.h>
#include "debug.h"
#include "common.h"

const char * tcp_state_str[] = {
    "",
    "TCP_ESTABLISHED",
    "TCP_SYN_SENT",
    "TCP_SYN_RECV",
    "TCP_FIN_WAIT1",
    "TCP_FIN_WAIT2",
    "TCP_TIME_WAIT",
    "TCP_CLOSE",
    "TCP_CLOSE_WAIT",
    "TCP_LAST_ACK",
    "TCP_LISTEN",
    "TCP_CLOSING"
};

int main(int argc, char ** argv)
{
    Assert(argc == 2, "param lack");
    int n = atoi(argv[1]);
    char c_msg[] = "Hello Server";
    char s_msg[] = "Hello Client";
    char c_buf[64], s_buf[64];

    int pid1, pid2;
    pid1 = fork();
    if(pid1 == 0)
    {
        // 子进程 1 作为服务端
        int fd = server(9000), clnt_fd, clnt_addr_size;
        Assert(fd != -1, "server() error");
        int t;
        sockaddr_in clnt_addr;
        clnt_fd = accept(fd, (sockaddr*)&clnt_addr, &clnt_addr_size);
        if(clnt_fd == -1)
            FATAL("accept() error");
        memset(s_buf, 0, 64);
        while(recv(clnt_fd, s_buf, 64, 0) > 0)
        {
            printf("Server: %s\n", s_buf);
            send(clnt_fd, s_msg, sizeof(s_msg), 0);
        }
    }
    else if(pid1 > 0)
    {
        pid2 = fork();
        if(pid2 == 0)
        {
            // 子进程 2 作为客户端
            int remote_fd = c_connect("localhost", 9000);
            Assert(remote_fd != -1, "c_connect() error");
            // while(1)
            for(int i = 1; i <= n; i++)
            {
                send(remote_fd, c_msg, sizeof(c_msg), 0);
                memset(c_buf, 0, 64);
                recv(remote_fd, c_buf, 64, 0);
                printf("%d: %s\n",i, c_buf);
            }
        }
        else if(pid2 > 0)
        {   
            INFO("child %d has been created\n", pid1);
            INFO("child %d has been created\n", pid2);
            INFO("wait child...\n");
            waitpid(pid1, NULL, 0);
            waitpid(pid2, NULL, 0);
            INFO("parent process exit\n");
        }
        else
            FATAL("fork() error 2");
    }
    else
        FATAL("fork() error 1");

    return 0;
}