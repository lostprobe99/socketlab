/**
 * check_system.c
 *
 * @author : lostprobe
 * @date   : 2023/06/04 10:28
 * @brief  : check_system.c
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "util.h"

void test_itf_get()
{
    printf("sockaddr_ll  size: %zu\n", sizeof(struct sockaddr_ll));
    printf("sockaddr     size: %zu\n", sizeof(struct sockaddr));
    printf("sockaddr_in  size: %zu\n", sizeof(struct sockaddr_in));
    printf("sockaddr_in6 size: %zu\n", sizeof(struct sockaddr_in6));

    struct sockaddr_ll hwaddr = {0};
    get_itf_mac("eth0", &hwaddr);
    printf("网卡MAC: %02X:%02X:%02X:%02X:%02X:%02X\n", ALL_MAC_BYTE(hwaddr.sll_addr));

    struct sockaddr_in v4addr = {0};
    get_itf_ip4("eth0", &v4addr);
    printf("网卡IP: %s\n", inet_ntoa(v4addr.sin_addr));

    struct sockaddr_in subnet_mask = {0};
    get_itf_subnet_mask("eth0", &subnet_mask);
    printf("子网掩码: %s\n", inet_ntoa(subnet_mask.sin_addr));
}

void test_byte_seq()
{
    int32_t f = 0x12345678;
    int32_t f1 = 0;
    hexdump((uint8_t*)&f, sizeof(f));
    if(*(uint8_t*)&f == 1) {
        printf("小端\n");
    } else {
        printf("大端\n");
    }
}

void test_env_check()
{
    #ifdef IS_LINUX
        printf("is linux\n");
    #elif IS_WINDOWS
        printf("is windows\n");
    #endif

    #ifdef IS_GNU_C
        printf("is gnu c\n");
    #endif

    #ifdef IS_GNU_CXX
        printf("is gnu cxx\n");
    #endif
}

struct __attribute__((packed)) packed_struct {
    uint8_t a;
    uint32_t b;
    uint8_t c;
};

struct normal_struct {
    uint8_t a;
    uint32_t b;
    uint8_t c;
};

void test_packed_attr()
{
    printf("packed_struct size: %zu\n", sizeof(struct packed_struct));
    printf("normal_struct size: %zu\n", sizeof(struct normal_struct));
}

int main(int argc, char ** argv)
{
    test_packed_attr();
    test_byte_seq();
    test_itf_get();
    test_env_check();

    return 0;
}