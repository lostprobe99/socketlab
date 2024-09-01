/**
 * 
 * daemon.c
 * 
 * @author: lostprobe
 * @date  : 2024/07/07 20:35:07
 * @brief : daemon.c
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <syslog.h>

void daemonize() {
    pid_t pid, sid;

    /* 创建子进程，使父进程退出 */
    pid = fork();
    if (pid < 0) {
        exit(EXIT_FAILURE);
    }
    if (pid > 0) {
        /* 父进程退出 */
        exit(EXIT_SUCCESS);
    }

    /* 创建新会话 */
    sid = setsid();
    if (sid < 0) {
        /* 失败处理 */
        exit(EXIT_FAILURE);
    }

    /* 更改当前工作目录 */
    if ((chdir("/")) < 0) {
        /* 失败处理 */
        exit(EXIT_FAILURE);
    }

    /* 关闭标准文件描述符 */
    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);
}

int main() {
    openlog("mydaemon", LOG_PID, LOG_DAEMON); /* 初始化syslog */

    daemonize();

    while (1) {
        syslog(LOG_INFO, "Daemon is running...");
        sleep(10);
    }

    closelog(); /* 关闭syslog */
    return 0;
}
