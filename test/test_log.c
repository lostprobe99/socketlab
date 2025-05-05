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
    // test log mesg
    log_level_mesg(0, __FILE__, __FUNCTION__, __LINE__, "hello %s\n", "world");
    log_debug("hello %s\n", "world");
    log_info("hello %s\n", "world");
    log_warn("hello %s\n", "world");
    log_error("hello %s\n", "world");

    // test log level
    set_log_level(LOG_LEVEL_INFO);
    log_debug("hello %s\n", "world");
    log_info("hello %s\n", "world");
    log_warn("hello %s\n", "world");
    sleep(1);
    log_error("hello %s\n", "world");

    // test log_hexdump
    uint8_t mac[] = {0x80, 0x2d, 0x1a, 0x1e, 0x55, 0xde};
    log_hexdump(LOG_LEVEL_INFO, "mac", mac, sizeof(mac));

    // test log off
    set_log_level(LOG_LEVEL_OFF);
    log_hexdump(LOG_LEVEL_INFO, "mac", mac, sizeof(mac));

    // test log_perror
    set_log_level(LOG_LEVEL_DEBUG);
    int sock = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
    if (sock < 0) {
        log_perror("socket()");
    }

    log_die("hello %s\n", "world"); // exit 1
    log_info("hello %s\n", "world");

    return 0;
}