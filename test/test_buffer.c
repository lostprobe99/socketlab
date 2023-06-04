/**
 * buffer.c
 *
 * @author : lostprobe
 * @date   : 2023/06/03 22:48
 * @brief  : test_buffer.c
 */

#include "buffer.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char ** argv)
{
    BUFFERFREE buffer_t *buf = buffer_make(64);
    buf->len = read(0, buf->buf, buf->capacity);
    buf->buf[buf->len] = 0;
    printf("buf = %s", buf->buf);
    // buffer_free(&buf);

    return 0;
}