/**
 * buffer.c
 *
 * @author : lostprobe
 * @date   : 2023/06/03 23:04
 * @brief  : buffer.c
 */
#include "buffer.h"
#include <stdlib.h>
#include <string.h>

buffer_t* buffer_make(int capacity)
{
    buffer_t *buf = malloc(sizeof(buffer_t));
    if(buf == NULL)
        return NULL;
    buf->capacity = capacity;
    buf->buf = malloc(sizeof(char) * capacity);
    if(buf->buf == NULL)
    {
        free(buf);
        return NULL;
    }
    buf->len = 0;
    return buf;
}

int buffer_resize(buffer_t* buf, int capacity)
{
    char * old = buf->buf;
    buf->buf = malloc(sizeof(char) * capacity);
    if(buf->buf == NULL)
    {
        buf->buf = old;
        return 1;
    }
    memcpy(buf->buf, old, buf->len);
    free(old);
    buf->capacity = capacity;
    return 0;
}

void buffer_free(buffer_t **buf)
{
    free((*buf)->buf);
    free(*buf);
    *buf = NULL;
}

void buffer_clear(buffer_t *buf)
{
    memset(buf->buf, 0, buf->len);
}
