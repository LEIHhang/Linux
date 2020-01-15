#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
int main()
{
    close(1);
    int fd;
    fd = open("./tmp.txt",O_RDWR);//以可读可写方式打开文件,并获取他的文件描述符
    printf("这是一条数据\n");
    return 0;
}

