/**
 * shm2.c
 *
 * @author : lostprobe
 * @date   : 2025/09/14 18:40
 * @brief  : shm2.c
 */
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char ** argv)
{
    // 使用相同的键值获取已存在的共享内存段
    key_t key = ftok("shmfile", 65);
    // 获取已创建的共享内存段，不带 IPC_CREAT 标志
    int shmid = shmget(key, 1024, 0666);
    if (shmid == -1) {
        perror("shmget");
        exit(EXIT_FAILURE);
    }
    
    // 连接共享内存段到进程地址空间
    char *str = (char*) shmat(shmid, NULL, 0);
    if (str == (void*)-1) {
        perror("shmat");
        exit(EXIT_FAILURE);
    }
    
    printf("(%d) Data read from shared memory: %s\n", getpid(), str);
    
    // 分离共享内存段
    shmdt(str);
    return 0;
}