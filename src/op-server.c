#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include "common.h"
#include "debug.h"

#define BUF_SIZE 1024

typedef enum _color {
    RED = 1,
    GREEN = 2,
    BLUE = 4
}Color;
static const Color color = RED | GREEN;

static int add(int n, int *nums)
{
    int result = nums[0];
    for(int i = 1; i < n; i++)
        result += nums[i];
    return result;
}

static int sub(int  n, int *nums)
{
    int result = nums[0];
    for(int i = 1; i < n; i++)
        result -= nums[i];
    return result;
}

static int multi(int n, int *nums)
{
    int result = nums[0];
    for(int i = 0; i < n; i++)
        result *= nums[i];
    return result;
}

int calc(int n, int *nums, char op)
{
    int(*func[4])(int, int*);;
    func['*' - '*'] = multi;
    func['+' - '*'] = add;
    func['-' - '*'] = sub;
    return func[op - '*'](n, nums);
}

int main(int argc, char ** argv)
{
    if(argc != 2)
    {
        printf("Usage: %s <port>\n", argv[0]);
        exit(1);
    }
    int serv_sock, clnt_sock;
    char opmsg[BUF_SIZE];
    struct sockaddr_in serv_addr, clnt_addr;
    socklen_t clnt_addr_sz = sizeof(clnt_addr);

    serv_sock = socket(AF_INET, SOCK_STREAM, 0);
    if(serv_sock == -1)
        FATAL("socket");

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(atoi(argv[1]));
    serv_addr.sin_addr.s_addr = htons(INADDR_ANY);

    if(bind(serv_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1)
        FATAL("bind");
    if(listen(serv_sock, 5) == -1)
        FATAL("listen");

    int opnd_cnt, result;
    int recv_len, recv_cnt;
    for(int i = 0; i < 5; i++)
    {
        opnd_cnt = 0;
        clnt_sock = accept(serv_sock, (struct sockaddr*)&serv_addr, &clnt_addr_sz);
        read(clnt_sock, &opnd_cnt, 1);
        
        recv_len = 0;
        // 不能确保一次性读完
        while(recv_len < (opnd_cnt * sizeof(int) + 1))
            // BUF_SIZE - 1: 因为第一个字节以被读出
            recv_len += read(clnt_sock, opmsg + recv_len, BUF_SIZE - 1);
        result = calc(opnd_cnt, (int*)opmsg, opmsg[recv_len - 1]);
        write(clnt_sock, (char*)&result, sizeof(result));
        close(clnt_sock);
    }
    close(serv_sock);
    
    return 0;
}
