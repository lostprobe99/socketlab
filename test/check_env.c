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

int main(int argc, char ** argv)
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

    return 0;
}