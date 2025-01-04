/**
 * exec 函数族
 *
 * @author : lostprobe
 * @date   : 2025/01/04 12:57
 * @brief  : exec.c
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "util.h"

void test_execl(void)
{
    // int execl(const char *__path, const char *__arg, ...)
    // l: 将每个新程序的每个参数作为一个单独的参数, 以 NULL 结束
    // execl, execlp, execle
    printf("before execl\n");
    if(execl("/bin/echo", "echo", "hello", ", ", "world", "!", NULL) == -1)
    {
        perror("execl");
    }
    printf("after execl\n");
}

void test_execlp(void)
{
    // int execlp(const char *__file, const char *__arg, ...)
    // p: 如果__file 不存在, 则在 PATH 环境变量中搜索
    // execlp, execvp, execvpe
    printf("before execl\n");
    if(execl("ps", "-ef", NULL) == -1)
    {
        // 报错: No such file or directory
        perror("execl");
    }
    printf("after execl\n");

    printf("before execlp\n");
    if(execlp("ps", "-ef", NULL) == -1)
    {
        perror("execlp");
    }
    printf("after execlp\n");
}

void test_execv(void)
{
    // int execvp(const char *__file, char *const *__argv)
    // v: 将新程序的参数作为一个数组传递, 以 NULL 结束
    char *argv[] = {"ps", "-f", NULL};
    printf("before execvp\n");
    if(execvp("ps", argv) == -1)
    {
        perror("execvp");
    }
    printf("after execvp\n");
}

void test_execle(void)
{
    // int execle(const char *__path, const char *__arg, ...)
    // e: 使用 envp 指定的环境变量，而不是用当前环境
    char *envp[] = {"AA=aa", "BB=bb", NULL};
    printf("before execle\n");
    if(execle("/bin/sh", "sh", "-c", "echo $AA $BB", NULL, envp) == -1)
    {
        perror("execle");
    }
    printf("after execle\n");
}

void test_os_exec(void)
{
    int ret = 0;
    ret = os_exec("ls", "-l", "-a", NULL);
    printf("os_exec ret: %d\n", ret);
    ret = os_exec("echo", "hello", "world", NULL);
    printf("os_exec ret: %d\n", ret);
    ret = os_exec("11echo", "hello", "world", NULL);
    printf("os_exec ret: %d\n", ret);
}

int main(int argc, char ** argv)
{
    // test_execl();
    // test_execlp();
    // test_execv();
    // test_execle();
    test_os_exec();

    return 0;
}