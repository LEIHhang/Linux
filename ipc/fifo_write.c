#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
//int mkfifo(const char *pathname, mode_t mode);
//pathname:管道文件路径名称，mode:管道文件的操作权限
//成功返回0，失败返回-1
//这个demo用来演示命名管道的基本操作
int main()
{
    char *file = "./test.fifo";
    int ret=mkfifo(file,0664);
    if(ret<0){
        if(errno!=EEXIST){
        perror("mkfifo error");
        return -1;
        }
    }
    int fd = open(file,O_WRONLY);
    if(fd<0){
        perror("open error");
        return -1;
    }
    printf("write open sucess\n");
    char buf[1024];
    int i=0;
    while(1)
    {
        sprintf(buf,"我是第%d条数据\n",i++);
        write(fd,buf,strlen(buf));

    }
    return 0;
}

