/**
 * util.c
 *
 * @author : lostprobe
 * @date   : 2023/06/16 18:22
 * @brief  : util.c
 */

#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>

int round_two(int x)
{
    if (x == 0)
        return 1;
    if (x == 1)
        return 2;
    // x 是 2 的幂
    if ((x & (x - 1)) == 0)
        return x;
    int last;
    do
    {
        // 不断循环，保留最高位的 1
        last = x;
        x = x & (x - 1);
    } while (x != 0);

    // 最后左移 1
    return last << 1;
}

int systemf(const char * fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    int len = vsnprintf(NULL, 0, fmt, args);
    if(len <= 0)
    {
        va_end(args);
        perror("Error estimating buffer size");
        return -1;
    }
    char * cmd = malloc(len + 1);
    if(!cmd)
    {
        va_end(args);
        perror("Memory alloction failed");
        return -1;
    }
    vsnprintf(cmd, len + 1, fmt, args);
    va_end(args);

    int res = system(cmd);

    free(cmd);

    return res;
}