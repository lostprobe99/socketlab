#include <threads.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <assert.h>
#include <stdbool.h>

#define CAT(s1, s2) s1 "." s2

bool is_prime1(int n)
{
    for(int i = 2; i < n; i++)
    {
        if(n % i == 0)
            return false;
    }
    return true;
}

bool is_prime2(int n)
{
    for(int i = 2; i <= (int)sqrt(n); i++)
    {
        if(n % i == 0)
            return false;
    }
    return true;
}

bool is_prime3(int n)
{
    int bound = sqrt(n);
    for(int i = 2; i <= bound; i++)
    {
        if(n % i == 0)
            return false;
    }
    return true;
}

void prime(int n, bool (*is_prime)(int))
{
    int c = 0;
    for(int i = 2; i < n; i++)
    {
        if(is_prime(i))
            c++;
    }
    printf("%d\n", c);
}

int session(void * args)
{
    FILE * fp = fopen(CAT(__FILE__, "log"), "a");
    assert(("log open failed", fp == NULL));
    fprintf(fp, "子线程启动\n");

    prime(100000, is_prime1);

    printf("子线程退出\n");

    fclose(fp);

    return 0;
}

int main()
{
    thrd_t thr1;
    int s = 1000;
    int state = thrd_create(&thr1, session, NULL);
    assert(("thread create error", state == thrd_success));
    thrd_join(thr1, NULL);

    printf("thread[%s]: Sleep %ds\n", __func__, s);
    thrd_sleep(&(struct timespec){.tv_sec = s}, NULL);
    printf("主线程退出\n");

    return 0;
}