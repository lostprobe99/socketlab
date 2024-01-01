/**
 * tofd.c
 *
 * @author : lostprobe
 * @date   : 2023/06/16 15:50
 * @brief  : tofd.c
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include "debug.h"
#include "buffer.h"


int main(int argc, char ** argv)
{
    char buf1[] = "msg from write\n";
    char buf2[] = "msg from fprintf\n";
    int fd = open("tofd.tmp", O_RDWR | O_CREAT);
    Assert(fd != -1, "file open failed");
    INFO("fd of file `tofd.tmp` is %d\n", fd);
    write(fd, buf1, sizeof(buf1));

    FILE* fp = fdopen(fd, "w");
    fprintf(fp, "fp to fd\n");
    printf(EVAL(fileno(fp), "%d"));
    fprintf(fp, "%s", buf2);

    fclose(fp);
    close(fd);

    return 0;
}