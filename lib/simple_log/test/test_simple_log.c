/**
 * simple_log_test.c
 *
 * @author : lostprobe
 * @date   : 2025/09/01 21:17
 * @brief  : simple_log_test.c
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "simple_log.h"
#include "debug.h"

int test_sink()
{
    char *s = NULL;
    FILE* fp = NULL;

    fp = simple_log_get_log_os();
    INFO("sink fp: %p\n", fp);


    simple_log_set_log_os(stdout);
    fp = simple_log_get_log_os();
    INFO("sink fp: %p, fp == stdout: %d\n", fp, fp == stdout);

    return 0;
}

int main(int argc, char ** argv)
{
    test_sink();
    log_info("hello world\n");
    log_info("%%m is: %m\n");

    FILE* fp = fopen("test.log", "r");  // trigger an error
    log_info("fopen(): %m\n");
    log_info("%%m is: %%m\n");

    return 0;
}