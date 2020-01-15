#include <stdio.h>
#include <sys/shm.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/wait.h>
//int shmget(key_t key, size_t size, int shmflg);
//这个demo用来演示shmget的使用
#define IPC_KEY 0x12345678//设置共享内存标识
#define SHM_SIZE 4096//设置共享内存大小
int main()
{
    int shmid = shmget(IPC_KEY,SHM_SIZE,IPC_CREAT|0664);
    //0664设置共享内存权限
    if(shmid<0){
        perror("shmget error");;
        return -1;
    }
    //建立映射关系，第二个参时是设置共享内存映射首地址，但是我们一般设置为NULL，让操作系统设置一个合适的位置。
    //第三个参数是设置进程权限，设置SHM_RDONLY的前提是该文件必须要有可读权限。设置其他表示可读可写。
    //返回值：成功返回映射的首地址，失败返回(void*)-1
    void *shm_start = shmat(shmid,NULL,0);

    if(shm_start==(void *)-1){
        perror("shmat error");
        return -1;
    }
    int pid=fork();
    if(pid<0)
    {
        perror("fork error\n");
        return -1;
    }
    if(pid==0)
    {
        while(1)
        {
            shmat(shmid,NULL,0);
            printf("%s\n",shm_start);
            sleep(2);
        }
    }
    else
    {
        int i=0;
        while(1){
            sprintf((char*)shm_start,"我是第%d条数据",i++);
            sleep(1);
        }
    }
    //关闭映射关系，参数是映射首地址
    shmdt(shm_start);
    //关闭共享内存，第二个参数设置对共享内存的操作
    //第三个参数获取共享内存的一些属性
    shmctl(shmid,IPC_RMID,NULL);
    waitpid(pid,NULL,0);
    return 0;
}
