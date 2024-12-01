#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <errno.h>

#include <netinet/tcp.h>    /// for tcp_info
#include <dirent.h>
#include <signal.h>

#include <readline/readline.h>
#include <readline/history.h>

#include "common.h"
#include "debug.h"
#include "minish.h"
#include "ft.h"

#define BUF_SIZE 1024
#define HISTORY_FILE "~/.ft_history"

#define PRINT_RESPONSE_BODY                                \
    char *s, *se;                                          \
    int n;                                                 \
    do                                                     \
    {                                                      \
        if ((n = recv(server_fd, buf, BUF_SIZE, 0)) == -1) \
            DIE("recv() error");                    \
        s = buf;                                           \
        se = buf + n;                                      \
        while (s < se)                                     \
        {                                                  \
            if (*s == END)                                 \
            {                                              \
                putchar('\n');                             \
                return 0;                                  \
            }                                              \
            putchar(*s);                                   \
            s++;                                           \
        }                                                  \
    } while (1)

#define CONNECT_CHECK                         \
    if (tcp_connected(server_fd) == 0)        \
    {                                         \
        WARN("The server is not connected."); \
        return -1;                            \
    }

socket_t server_fd = 0;
char buf[BUF_SIZE];

int cmd_open(char * args);
int cmd_ls(char * args);
int cmd_get(char * args);
int cmd_put(char * args);
int cmd_exit(char * args);
int cmd_help(char * args);
int cmd_pwd(char * args);
int cmd_cd(char * args);


cmd_entry cmd_table[] = {
    "open", "open <host> <port>, open a ft server", cmd_open,
    "ls", "ls, list directory contents", cmd_ls,
    "get", "get <filename>, download file", cmd_get,
    "put", "put <filename>, upload file", cmd_put,
    "help", "help, display help information", cmd_help,
    "pwd", "pwd, print current directory", cmd_pwd,
    "cd", "cd <path>, change current directory to <path>", cmd_cd,
    "exit", "client exit", cmd_exit
};
cmd_config ftc = {.prompt = "ft> ", .cmd_table = cmd_table, .cmd_table_len = sizeof(cmd_table) / sizeof(cmd_table[0]) };

int recv_file(int fd, const char * filename, const long filesize, int recv_len)
{
    int n = 0;
    long t = 0;
    // 文件大小(8) 文件数据
    char * s = buf + sizeof(long) + 1;
    FILE *fp = fopen(filename, "wb");
    n = fwrite(s, 1, recv_len - sizeof(long) - 1, fp);
    t += n;
    printf("%s: %ld/%ld %.2lf%%\r", filename, t, filesize, 1.0 * t / filesize * 100);
    while(t < filesize)
    {
        memset(buf, 0, BUF_SIZE);
        n = recv(fd, buf, BUF_SIZE, 0);
        t += n;
        printf("%s: %ld/%ld %.2lf%%\r", filename, t, filesize, 1.0 * t / filesize * 100);
        fwrite(buf, 1, n, fp);  // 写 n 次 1 字节
    }
    fclose(fp);
    printf("\nrecv %s done\n", filename);
}

// OK
// 打开一个连接
int cmd_open(char * args)
{
    // TODO: 关闭已打开的连接
    struct tcp_info info;
    
    char * addr = strtok(NULL, " ");
    char * port = strtok(NULL, " ");
    if(addr == NULL || port == NULL)
    {
        ERROR("not complete addr");
        return 0;
    }
    server_fd = make_socket(AF_INET, SOCK_STREAM, 0);
    Assert(server_fd != INVALID_SOCKET, "socket() error");
    sockaddr_in serv_addr = make_sockaddr(AF_INET, addr, atoi(port));
    if(connect(server_fd, (sockaddr*)&serv_addr, sizeof(serv_addr)) == SOCKET_ERROR)
        WARN("connect %s:%s failed", addr, port);
    else
        printf("%s:%s connected\n", addr, port);

    return 0;
}

// OK
// 查看远程主机的文件
int cmd_ls(char * args)
{
    CONNECT_CHECK;
    struct tcp_info info;
    if(get_tcp_info(server_fd, &info) == 0)
        INFO("fd = %d, TCP State: %d [%s]", server_fd, info.tcpi_state, _tcp_state_str_[info.tcpi_state]);
    memset(buf, 0, BUF_SIZE);
    buf[0] = LS;
    if(args != NULL)
        strncpy(buf + 1, args, BUF_SIZE - 1);
    send(server_fd, buf, BUF_SIZE, 0);
    memset(buf, 0, BUF_SIZE);
    PRINT_RESPONSE_BODY;
    
    return 0;
}

// 下载文件
int cmd_get(char * args)
{
    CONNECT_CHECK;
    long fsize = 0;
    int n = 0;
    char * s;
    memset(buf, 0, BUF_SIZE);
    if(args != NULL)
    {
        buf[0] = GET;   // 单字节无需转换
        strcpy(buf + 1, args);
        // 发送请求和文件名
        send(server_fd, buf, BUF_SIZE, 0);
        memset(buf, 0, BUF_SIZE);
        // 接收文件大小
        n = recv(server_fd, buf, BUF_SIZE, 0);  // 文件大小和文件数据会在此处一同接收，导致在 recv_file 中丢失数据从而等待 recv 的情况
        // 接收文件状态信息
        if(buf[0] == ERROR) // 第 0 个字节描述是否出错
        {
            printf("%s\n", buf + 1);
            return -1;
        }
        // 如何划分数据边界 --> 粘包问题
        fsize = ntohl(*(long*)(buf + 1));
        printf("file size: %ld bytes\n", fsize);
        recv_file(server_fd, args, fsize, n);
        return 0;
    }
    WARN("file name not given");

    return 0;
}

int cmd_put(char * args)
{
    CONNECT_CHECK;
    long fsize = 0;
    int n = 0, offset = 0;
    memset(buf, 0, BUF_SIZE);
    if(args != NULL)
    {
        // put(1) 文件大小(8) 文件名长度(4) 文件名
        char * filename = strtok(NULL, " ");
        buf[0] = PUT;
        offset += 1;
        fsize = get_file_size(filename);
        *(long*)(buf + offset) = htonl(fsize);   // 保存文件大小
        offset += sizeof(long);
        n = strlen(filename);
        *(int*)(buf + offset) = htonl(n);
        offset += sizeof(int);
        strncpy(buf + offset, filename, n);
        printf("buf decode: %s %ld %d %s\n", ft_request_str[buf[0]], (long)ntohl(*(long*)(buf + 1)), (int)ntohl(*(int*)(buf + 1 + sizeof(long))), buf + 1 + sizeof(long) + sizeof(int));
        send(server_fd, buf, offset + n, 0);
        // TODO: 检查指针
        FILE* fp = fopen(filename, "rb");
        // 发送文件体
        while((n = fread(buf, 1, BUF_SIZE, fp)) != 0)
            send(server_fd, buf, n, 0);
        fclose(fp);
        recv(server_fd, buf, 1, 0);
        if(buf[0] == END)
            printf("send `%s` successful\n", filename);
        return 0;
    }
    WARN("file name not given");

    return -1;
}

int cmd_exit(char * args)
{
    memset(buf, 0, BUF_SIZE);
    buf[0] = EXIT;
    send(server_fd, buf, BUF_SIZE, 0);
    INFO("`EXIT` send");
    // 清理已分配的资源
    close(server_fd);
    exit(0);
}

void sig_exit(int sig)
{
    if(sig == SIGINT)
    {
        cmd_exit(NULL);
    }
}

// OK
int cmd_help(char * args)
{
    char * arg = strtok(NULL, " ");
    if(arg == NULL)
    {
        for(int i = 0; i < ftc.cmd_table_len; i++)
            printf("%s - %s\n", ftc.cmd_table[i].name, ftc.cmd_table[i].desc);
    }
    else
    {
        for(int i = 0; i < ftc.cmd_table_len; i++)
        {
            if(strcmp(arg, ftc.cmd_table[i].name) == 0)
            {
                printf("%s - %s\n", ftc.cmd_table[i].name, ftc.cmd_table[i].desc);
                return 0;
            }
        }
        printf(ANSI_FMT("Unknown command `%s`\n", ANSI_FG_RED), arg);
    }
}

// OK
// 打印当前所在目录
int cmd_pwd(char * args)
{
    CONNECT_CHECK;
    memset(buf, 0, BUF_SIZE);
    buf[0] = PWD;
    send(server_fd, buf, 1, 0);
    memset(buf, 0, BUF_SIZE);
    PRINT_RESPONSE_BODY;

    return 0;
}

// OK
// 切换到目标目录
int cmd_cd(char * args)
{
    CONNECT_CHECK;
    memset(buf, 0, BUF_SIZE);
    buf[0] = CD;
    if(args != NULL)
        strncpy(buf + 1, args, BUF_SIZE - 1);
    send(server_fd, buf, BUF_SIZE, 0);
    PRINT_RESPONSE_BODY;

    return 0;
}

int main(int argc, char **argv)
{
    // if(access(HISTORY_FILE, W_OK | R_OK) == -1)
    // {
    //     remove(HISTORY_FILE);
    //     FILE * fh = fopen(HISTORY_FILE, "w+");
    //     if(fh == NULL)
    //     {
    //         perror("fopen() error");
    //         exit(1);
    //     }
    //     fclose(fh);
    // }
    // if(read_history(HISTORY_FILE) != 0)
    // {
    //     perror(strerror(errno));
    //     ERROR("read_history() error");
    // }

    signal(SIGINT, sig_exit);   // 注册 <C-c> 为 cmd_exit

    if (argc == 3)  // 尝试连接给定的主机
    {
        char * addr = argv[1], *port = argv[2];
        server_fd = c_connect(addr, atoi(port));
        Assert(server_fd != -1, "c_connect() error");
        printf("%s:%s connected\n", addr, port);
    }

    printf("Type \"help\" for help\n");

    minish(&ftc);

    // write_history(HISTORY_FILE);

    return 0;
}