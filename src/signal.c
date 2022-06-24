/**
 * signal.c
 *
 * @Author : lostprobe
 * @Date   : 2022/03/01 18:23
 * @Brief  : signal.c
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>

#include "common.h"

void keyctrl(int sig)
{
    if(sig == SIGINT)
    {
        printf("\nCtrl+C\n");
        exit(0);
    }
}

int main(int argc, char ** argv)
{
    signal(SIGINT, keyctrl);
    // 发送 ctrl+c 时，会自动调用 keyctrl
    for (int i = 0; i < 3; i++)
    {
        printf("%d\n", i);
        sleep(100000);
    }

    return 0;
}