/**
 * buffer.h
 *
 * @author : lostprobe
 * @date   : 2023/06/03 23:05
 * @brief  : buffer.h
 */

#pragma once

typedef struct {
    char *buf;
    int len;
    int capacity;
} buffer_t;

#if defined(__GNUC__)
#define BUFFERFREE __attribute__((__cleanup__(buffer_free)))
#else
#define BUFFERFREE assert(0)
#endif

buffer_t* buffer_make(int capacity);
/// @brief 调整一个 buffer 的容量
/// @param buf 
/// @param capacity 新的容量
/// @retval 0 - 成功
/// @retval 1 - 失败
int buffer_resize(buffer_t *buf, int capacity);
void buffer_free(buffer_t **buf);
void buffer_clear(buffer_t *buf);