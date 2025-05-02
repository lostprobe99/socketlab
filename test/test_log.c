/**
 * test_log.c
 *
 * @author : lostprobe
 * @date   : 2024/11/29 23:31
 * @brief  : test_log.c
 */

#include "util.h"
#include "simple_log.h"

int main(int argc, char ** argv)
{
    printf("%s\n", log_get_time());
    log_mesg(0, __FILE__, __LINE__, "hello %s\n", "world");
    LOG_DEBUG("hello %s\n", "world");
    LOG_INFO("hello %s\n", "world");
    LOG_WARN("hello %s\n", "world");
    LOG_ERROR("hello %s\n", "world");

    set_log_level(LOG_INFO);

    LOG_DEBUG("hello %s\n", "world");
    LOG_INFO("hello %s\n", "world");
    LOG_WARN("hello %s\n", "world");
    sleep(1);
    LOG_ERROR("hello %s\n", "world");

    DIE("hello %s\n", "world"); // exit 1
    LOG_INFO("hello %s\n", "world");

    return 0;
}