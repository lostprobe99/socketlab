/**
 * simple_log_time.c
 *
 * @author : lostprobe
 * @date   : 2025/09/01 22:41
 * @brief  : simple_log_time.c
 */

#include "simple_log.h"
#include "simple_log_time.h"

const char * slog_get_time()
{
    time_t t;
    struct tm *tmp;
    static char buf[64];
    struct timeval tv;
    int ms;

    t = time(NULL);
    tmp = localtime(&t);
    /* 获取当前时间，并根据微秒计算毫秒数 */
    gettimeofday(&tv, NULL);
    ms = tv.tv_usec / 1000;

    /* 格式化时间：年-月-日T时:分:秒 */
    strftime(buf, sizeof(buf), "%Y-%m-%dT%H:%M:%S", tmp);
    /* 在时间字符串末尾追加毫秒部分，格式为.毫秒(补全为3位) */
    snprintf(buf + strlen(buf), sizeof(buf) - strlen(buf), ".%03d", ms);

    return buf;
}