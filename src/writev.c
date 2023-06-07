/**
 * writev.c
 *
 * @author : lostprobe
 * @date   : 2023/06/04 15:49
 * @brief  : writev.c
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/uio.h>

int main(int argc, char ** argv)
{
    struct iovec vec[2];
    char buf1[] = "ABCDEFGHIJ";
    char buf2[] = "1234567890";
    vec[0] .iov_base = buf1;
    vec[0].iov_len = 3;
    vec[1].iov_base = buf2;
    vec[1].iov_len = 4;
    int str_len = writev(1, vec, 2);
    printf("\nWrite bytes: %d\n", str_len);

    return 0;
}