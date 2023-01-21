#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>

#include "common.h"

#define BUF_SIZE 1024

void childproc(int sig)
{
#ifdef linux
    int status;
    pid_t id = waitpid(-1, &status, WNOHANG);
    if (WIFEXITED(status))
        printf("Child process %d exited with status %d\n", id, WEXITSTATUS(status));
#endif
}

int main(int argc, char **argv)
{
    if (argc < 2)
    {
        printf("Usage: %s <port>\n", argv[0]);
        exit(1);
    }
    #ifdef __linux
    signal(SIGCHLD, childproc);

    int serv_sock = socket(AF_INET, SOCK_STREAM, 0), clnt_sock;
    if (serv_sock == -1)
        ERROR_HANDLING("socket");
    sockaddr_in serv_addr, clnt_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(atoi(argv[1]));

    if (bind(serv_sock, (sockaddr *)&serv_addr, sizeof(serv_addr)) == -1)
        ERROR_HANDLING("bind");
    if (listen(serv_sock, 5) == -1)
        ERROR_HANDLING("listen");

    pid_t pid;
    char msg[BUF_SIZE];
    int str_len, addr_sz;
    while (1)
    {
        clnt_sock = accept(serv_sock, (sockaddr *)&clnt_addr, &addr_sz);
        if (clnt_sock == -1)
        {
            ERROR_P("accept");
            continue;
        }
        else
            printf("Connected client %s:%d\n", inet_ntoa(clnt_addr.sin_addr), ntohs(clnt_addr.sin_port));

        pid = fork();
        if (pid == 0)
        {
            // 子进程
            close(serv_sock);
            while ((str_len = read(clnt_sock, msg, BUF_SIZE)) != 0)
                write(clnt_sock, msg, str_len);
            close(clnt_sock);
            return 0;
        }
        else if (pid > 0)
            close(clnt_sock);
        else
            ERROR_HANDLING("fork");
    }
    close(serv_sock);
    #endif

    return 0;
}