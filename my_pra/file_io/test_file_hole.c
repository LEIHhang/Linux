#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#define FILE_MODE 777
char buf1[]="woaini";
char buf2[]="zhongguo";
int main()
{
    int fd;
    if((fd=creat("file.hole",FILE_MODE)) < 0 )
        perror("文件创建失败:");
    if(write(fd,buf1,6)!=6)
        perror("向文件写入失败:");
    if(lseek(fd,20,SEEK_SET)==-1)
        perror("lseek失败:");
    if(write(fd,buf2,8)!=8)
        perror("文件写入失败2:");
    return 0;
}
//总结：lseek可以偏移到文件末尾之后的地方，并且在新开始的地方和之前结束的地方之间形成一个文件黑洞
//文件黑洞用/0填充，并不占用实际的磁盘大小

