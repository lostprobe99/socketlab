#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <dirent.h>
#include <errno.h>

#include "common.h"
#include "debug.h"
#include "ft.h"

#define BUF_SIZE 1024

char msg[BUF_SIZE], buf[BUF_SIZE];

// OK
int response_ls(socket_t fd, char * args)
{
    char * path;
    int n = 0;
    if(*args == 0)
        path = ".";
    else
        path = strtok(args, " ");
    DIR * dir;
    if((dir = opendir(path)) == 0)
    {
        memset(msg, 0, BUF_SIZE);
        n = sprintf(msg, "open dir `%s` failed: %s", path, strerror(errno));    // path 和 buf 在同一块内存
        send(fd, msg, n, 0);
        msg[0] = END;
        send(fd, msg, 1, 0);
        // perror("dir failure");
        return 1;
    }
    struct dirent * dirinfo;
    while(1)
    {
        if((dirinfo = readdir(dir)) == 0)
            break;
        if(dirinfo->d_name[0] == '.')
            continue;
        // 虽然是分开发送的，但是接收时会装到一个 buf 里
        n = sprintf(buf, "%s\n", dirinfo->d_name);
        send(fd, buf, n, 0);
    }
    buf[0] = END;
    send(fd, buf, 1, 0);    // 包括这个
    closedir(dir);
    return 0;
}

// OK
int response_pwd(socket_t fd, char * args)
{
    char * s;
    s = getcwd(NULL, 0);
    send(fd, s, strlen(s), 0);
    buf[0] = END;
    send(fd, buf, 1, 0);

    free(s);
    return 0;
}

int response_exit(socket_t fd, char * args)
{
    // 如何处理？
    // 关闭线程？

    close(fd);
    exit(0);
}

// OK
int response_cd(socket_t fd, char * args)
{
    char * path, *s;
    int n = 0;
    if(*args == 0)
        path = ".";
    else
        path = strtok(args, " ");

    if(chdir(path) == -1)
    {
        memset(msg, 0, BUF_SIZE);
        n = sprintf(msg, "cd `%s` failed: %s\n", path, strerror(errno));
        send(fd, msg, n, 0);
        response_pwd(fd, path);
    }
    else
        response_pwd(fd, path);

    return 0;
}

int response_get(socket_t fd, char * args)
{
    memset(msg, 0, BUF_SIZE);
    char * filename = strtok(args, " ");
    int n = 0;
    if(access(filename, R_OK) == -1)
    {
        msg[0] = ERROR;
        n = sprintf(msg + 1,"get `%s` failed: %s", filename, strerror(errno));
        send(fd, msg, 1 + n, 0);
        return -1;
    }
    msg[0] = DATA;
    long fsize = get_file_size(filename);
    *(uint32_t*)(msg + 1) = htonl(fsize);
    n = sizeof(long);   // 前 sizeof(long) 个字节为文件大小
    send(fd, msg, 1 + n, 0);
    printf("file: %s, size: %ld\n", filename, fsize);
    FILE* fp = fopen(filename, "rb");
    while((n = fread(msg, 1, BUF_SIZE, fp)) != 0)
        send(fd, msg, n, 0);
    fclose(fp);

    return 0;
}

static inline long min(long x, long y)
{
    return x < y ? x : y;
}

int response_put(socket_t fd, char * args)
{
    long t = 0, n = *(int*)args;
    int buf_offset = 1;
    // 解析文件大小
    long fsize = ntohl(*(long*)(buf + buf_offset));
    buf_offset += sizeof(long);
    int fnlen = ntohl(*(int*)(buf + buf_offset));
    buf_offset += sizeof(int);
    // 解析文件名
    char filename[256] = {0};
    strncpy(filename, buf + buf_offset, fnlen);
    buf_offset += fnlen;
    printf("file: %s, size: %ld\n", filename, fsize);
    FILE* fp = fopen(filename, "wb");
    // 将 buf 中剩余的数据写入文件
    t += fwrite(buf + buf_offset + 1, 1, n - buf_offset - 1, fp);
    while(t < fsize)
    {
        // 接收剩余的文件数据并保存
        n = recv(fd, buf, BUF_SIZE, 0);
        t += n;
        fwrite(buf, 1, n, fp);
    }
    fclose(fp);
    buf[0] = END;
    send(fd, buf, 1, 0);
    return 0;
}

int main(int argc, char ** argv)
{
    char * port = "9000";
    // if(argc != 2)
    // {
    //     printf("Usage: %s <port>\n", argv[0]);
    //     return -1;
    // }
    socket_t hServerSocket = make_socket(AF_INET, SOCK_STREAM, 0), hClientSocket;
    Assert(hServerSocket != INVALID_SOCKET, "socket() error");
    
    // 设置 SO_REUSEADDR
    int sock_opt = 1, optlen = sizeof(sock_opt);
    setsockopt(hServerSocket, SOL_SOCKET, SO_REUSEADDR, (void*)&sock_opt, optlen);

    sockaddr_in servAddr = make_sockaddr(AF_INET, "127.0.0.1", atoi(port));
    servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    sockaddr_in clntAddr;

    Assert(bind(hServerSocket, (sockaddr *)&servAddr, sizeof(servAddr)) != SOCKET_ERROR, "bind() error");

    Assert(listen(hServerSocket, 5) != SOCKET_ERROR, "listen() error");
    
    printf("Server starting at %s...\n", argv[1]);
    
    int n = 0, clnt_addr_size;

    // TODO: 多线程处理客户端
    hClientSocket = accept(hServerSocket, (sockaddr *)&clntAddr, &clnt_addr_size);
    Assert(hClientSocket != -1, "accept() error");
    printf("Got %s\n", inet_ntoa(clntAddr.sin_addr));   // 解析客户端 ip

    while(1)
    {
        // 接收请求，解析请求类型
        n = recv(hClientSocket, buf, BUF_SIZE, 0);
        buf[n] = 0;
        printf("Got %s\n", ft_request_str[buf[0]]);
        printf("args = `%s`\n", buf + 1);
        switch(buf[0])
        {
            case LS:
                response_ls(hClientSocket, buf + 1);    // OK
                break;
            case PWD:
                response_pwd(hClientSocket, buf + 1);   // OK
                break;
            case CD:
                response_cd(hClientSocket, buf + 1);    // OK
                break;
            case GET:
                response_get(hClientSocket, buf + 1);
                break;
            case PUT:
                response_put(hClientSocket, (char*)&n);
                break;
            case EXIT:
                response_exit(hClientSocket, buf + 1);  // ？？我很疑惑
                break;
            default:
                break;
        }
    }

    return 0;
}