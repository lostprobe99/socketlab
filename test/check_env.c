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

int main(int argc, char ** argv)
{
    sizeof(struct sockaddr_ll);
    sizeof(struct sockaddr);
    sizeof(struct sockaddr_in);
    sizeof(struct sockaddr_in6);

    struct sockaddr_ll hwaddr = {0};
    get_itf_mac("eth0", &hwaddr);
    printf("网卡MAC: %02X:%02X:%02X:%02X:%02X:%02X\n", ALL_MAC_BYTE(hwaddr.sll_addr));

    struct sockaddr_in v4addr = {0};
    get_itf_ip4("eth0", &v4addr);
    printf("网卡IP: %s\n", inet_ntoa(v4addr.sin_addr));

    struct sockaddr_in subnet_mask = {0};
    get_itf_subnet_mask("eth0", &subnet_mask);
    printf("子网掩码: %s\n", inet_ntoa(subnet_mask.sin_addr));

    int32_t f = 0x12345678;
    int32_t f1 = 0;
    hexdump((uint8_t*)&f, sizeof(f));
    if(*(uint8_t*)&f == 1) {
        printf("小端\n");
    } else {
        printf("大端\n");
    }

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

    return 0;
}