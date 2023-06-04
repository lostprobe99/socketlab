/**
 * cleanup.c
 *
 * @author : lostprobe
 * @date   : 2023/06/03 19:12
 * @brief  : cleanup.c
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void clean_up(int * final_value)
{
    printf("Cleaning up\n");
    printf("Final value: %d\n", *final_value);
}

// void *p 表示接收任意类型的参数
void auto_free(void *p)
{
    // 实际上得到的参数是变量的地址
    void **q = (void**)p;
    printf("auto free *q[%p]\n", *q);
    free(*q);
}

#define AUTOFREE __attribute__((__cleanup__(auto_free)))

int main(int argc, char ** argv)
{
    int n __attribute__((__cleanup__(clean_up))) = 1;
    n = 5;

    AUTOFREE char * p = malloc(16 * sizeof(char));
    memset(p, 8 * sizeof(char), 0);
    printf("allocate at %p\n", p);

    return 0;
}