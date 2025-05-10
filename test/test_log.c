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
    // test log mesg, default level is LOG_LEVEL_INFO
    log_level_mesg(LOG_LEVEL_DEBUG, __FILE__, __FUNCTION__, __LINE__, "hello %s\n", "world");
    log_debug("hello %s\n", "world");
    log_info("hello %s\n", "world");
    log_warn("hello %s\n", "world");
    log_error("hello %s\n", "world");
    log_fatal("hello %s\n", "world");

    // test log level
    printf("set log level to debug...\n");
    set_log_level(LOG_LEVEL_DEBUG);
    log_debug("hello %s\n", "world");
    log_info("hello %s\n", "world");
    log_warn("hello %s\n", "world");
    log_error("hello %s\n", "world");
    log_fatal("hello %s\n", "world");

    printf("set log level to warn...\n");
    set_log_level(LOG_LEVEL_WARN);
    log_debug("hello %s\n", "world");
    log_info("hello %s\n", "world");
    log_warn("hello %s\n", "world");
    log_error("hello %s\n", "world");
    log_fatal("hello %s\n", "world");

    printf("set log level to error...\n");
    set_log_level(LOG_LEVEL_ERROR);
    log_debug("hello %s\n", "world");
    log_info("hello %s\n", "world");
    log_warn("hello %s\n", "world");
    log_error("hello %s\n", "world");
    log_fatal("hello %s\n", "world");

    printf("sleeping...\n");
    sleep(1);

    // test log_hexdump
    uint8_t hex[] = {
        0x27,0xbb,0x36,0xc0,0x5f,0x64,0x8a,0x44,
        0xfa,0x60,0x72,0x0c,0x4f,0x9e,0x34,0x69,
        0x03,0x45,0xae,0x2c,0x9c,0x7b,0xc0,0x09,
        0xac,0xd0,0xf7,0x1b,0x90,0x89,0x07,0x58,
        0xd5,0x02,0xe6,0x88,0xc1,0x7b,0x94,0xf7,
        0x18,0x8a,0x62,0x1c,0xf0,0xfc,0x61,0xb7,
        0x35,0xa6,0x17,0x78,0x15,0x27,0x46,0x65,
        0x78,0x53,0x3f,0x08,0x96,0x7f,0x87,0xfc,
    };
    log_hexdump(LOG_LEVEL_DEBUG, "hex", hex, sizeof(hex));
    log_hexdump(LOG_LEVEL_INFO, "hex", hex, sizeof(hex));
    log_hexdump(LOG_LEVEL_WARN, "hex", hex, sizeof(hex));
    log_hexdump(LOG_LEVEL_ERROR, "hex", hex, sizeof(hex));
    log_hexdump(LOG_LEVEL_FATAL, "hex", hex, sizeof(hex));

    // test log off
    printf("set log level to off...\n");
    set_log_level(LOG_LEVEL_OFF);
    log_hexdump(LOG_LEVEL_DEBUG, "hex", hex, sizeof(hex));
    log_hexdump(LOG_LEVEL_INFO, "hex", hex, sizeof(hex));
    log_hexdump(LOG_LEVEL_WARN, "hex", hex, sizeof(hex));
    log_hexdump(LOG_LEVEL_ERROR, "hex", hex, sizeof(hex));
    log_hexdump(LOG_LEVEL_FATAL, "hex", hex, sizeof(hex));

    // test log_perror
    printf("set log level to fatal...\n");
    set_log_level(LOG_LEVEL_FATAL);
    int sock = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
    if (sock < 0) {
        log_perror(LOG_LEVEL_FATAL, "socket()");
        log_perror(LOG_LEVEL_ERROR, "socket()");
    }

    log_die("hello %s\n", "world"); // exit 1
    log_info("hello %s\n", "world");

    return 0;
}