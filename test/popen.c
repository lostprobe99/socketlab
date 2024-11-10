/**
 * popen.c
 *
 * @author : lostprobe
 * @date   : 2024/11/10 15:44
 * @brief  : popen demo
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char ** argv)
{
    char *s = "echo 456 | tee ~/repo/socketlab/log && cat ~/repo/socketlab/log";
    printf("使用 popen 运行 '%s' 命令并获取结果：\n", s);
    FILE *fp;
    char path[1035];

    fp = popen(s, "r");
    if (fp == NULL) {
        printf("无法执行命令\n");
        exit(1);
    }

    while (fgets(path, sizeof(path), fp) != NULL) {
        printf("%s", path);
    }
    pclose(fp);

    return 0;
}