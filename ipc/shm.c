#include <stdio.h>
#include <sys/shm.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <sys/ipc.h>
//int shmget(key_t key, size_t size, int shmflg);
//这个demo用来演示shmget的使用
#define IPC_KEY 0x12345678
#define SHM_SIZE 4096
int main()
{
    int shmid = shmget(IPC_KEY,SHM_SIZE,IPC_CREAT|0664);
    if(shmid<0){
        perror("shmget error");;
        return -1;
    }
    void *shm_start = shmat(shmid,NULL,0);
    if(shm_start==(void *)-1){
        perror("shmat error");
        return -1;
    }
    int i=0;
    while(1){
        sprintf(shm_start,"%s-%d\n","今天适合打篮球",i++);
        sleep(1);
    }
    shmdt(shm_start);
    shmctl(shmid,IPC_RMID,NULL);
    return 0;
}

