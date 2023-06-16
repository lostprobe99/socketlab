/**
 * buffer.c
 *
 * @author : lostprobe
 * @date   : 2023/06/03 22:48
 * @brief  : test_buffer.c
 */

#include "buffer.h"
#include "debug.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char ** argv)
{
    BUFFERFREE buffer_t *buf = buffer_make(64);
    // buf->len = read(0, buf->buf, buf->capacity);
    buf->len = strlen(strcpy(buf->buf, "Linking C"));
    buf->buf[buf->len] = 0;
    printf(EVAL(buf->len, "%d"));
    printf(EVAL(buf->capacity, "%d"));
    printf(EVAL(buf->buf, "`%s`"));
    // buffer_free(&buf);

    BUFFERFREE buffer_t* buf2 = buffer_make_s("Hello, World.");
    printf(EVAL(buf2->len, "%d"));
    printf(EVAL(buf2->capacity, "%d"));
    printf(EVAL(buf2->buf, "`%s`"));

    return 0;
}