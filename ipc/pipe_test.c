//本demo用来演示管道的应用实现和读写测试
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
int main()
{
    int fildes[2];
    int ret = pipe(fildes);
    if(ret<0)
    {
        perror("pipe create error\n");
        return -1;
    }
    int pid = fork();
    if(pid < 0)
    {
        perror("fork error\n");
        return -1;
    }
    if(pid == 0)
    {
        close(fildes[1]);//使用匿名管道时，哪一端不用就关闭它。
        while(1)
        {
            char buf[1024];
            read(fildes[0],buf,1024);
            printf("%s\n",buf);
        }
    }
    else
    {
        close(fildes[0]);//使用匿名管道时，哪一端不用就关闭它。
        int i=0;
        char buf[1024];
        while(1)
        {
            sleep(1);
            sprintf(buf,"这是第%d条数据\n",i++);
            write(fildes[1],buf,strlen(buf));
            printf("%s\n",buf);
        }
    }
    waitpid(pid,NULL,0);
    return 0;
}

