#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
int main()
{
    int fd;
    fd = open("./tmp.txt",O_RDWR|O_APPEND);
    dup2(fd,1);
    int fd2 = open("./tmp_2.txt",O_RDWR|O_CREAT|O_APPEND);
    //write(fd,"我是一条测试数据\n",25);
    //printf("%d\n",fd);
    printf("fd:%d, fd_2:%d\n",fd,fd2);
    write(fd,"我是一条数据\n",19);
    return 0;
}

