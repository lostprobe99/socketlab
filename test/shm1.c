/**
 * shm1.c
 *
 * @author : lostprobe
 * @date   : 2025/05/09 21:48
 * @brief  : shm1.c
 */
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char ** argv)
{
    // 使用 ftok 生成唯一的键值，'shmfile' 是文件路径，65 是子键
    key_t key = ftok("shmfile", 65);
    // 创建共享内存段：key=键值，1024=大小，0666=权限（读写），IPC_CREAT=不存在则创建
    int shmid = shmget(key, 1024, 0666|IPC_CREAT);
    if (shmid == -1) {
        perror("shmget");
        exit(EXIT_FAILURE);
    }
    
    // 将共享内存段附加到进程地址空间，NULL=自动选择地址，0=默认权限
    char *str = (char*) shmat(shmid, NULL, 0);
    if (str == (void*)-1) {
        perror("shmat");
        exit(EXIT_FAILURE);
    }
    
    // strcpy(str, "Hello from shm1");
    sprintf(str, "Hello from shm1(%d)", getpid());
    printf("Data written to shared memory (PID: %d)\n", getpid());
    
    // 将共享内存从进程地址空间分离
    shmdt(str);
    return 0;
}