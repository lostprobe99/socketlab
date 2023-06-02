/**
 * selec-echo-server.c
 *
 * @author : lostprobe
 * @date   : 2023/06/02 16:27
 * @brief  : selec-echo-server.c
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/select.h>
#include <sys/time.h>
#include "debug.h"
#include <unistd.h>

#define BUF_SIZE 1024

int main(int argc, char ** argv)
{
    fd_set reads, temps;
    FD_ZERO(&reads);
    FD_SET(0, &reads);  // 将 0 号 fd 加入监听, 0 号是标准输入
    struct timeval timeout;
    int result;
    char buf[BUF_SIZE];

    while(1)
    {
        temps = reads;
        timeout.tv_sec = 5;
        timeout.tv_usec = 0;
        result = select(1, &temps, NULL, NULL, &timeout);
        errif(result == -1, "select() error")
        else if(result == 0)
            puts("select: Timeout");
        else
        {
            if(FD_ISSET(0, &temps))
            {
                int len = read(0, buf, BUF_SIZE);   // 读入的数据含换行符
                buf[len] = 0;
                printf("console: %s", buf);
            }
        }
    }

    return 0;
}