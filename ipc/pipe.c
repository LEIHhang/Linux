#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <error.h>
#include <stdlib.h>
int main()
{
    int pipefd[2];
    int ret = pipe(pipefd);//创建匿名管道
    if(ret<0)
    {
        perror("pipe error\n");
        return -1;
    }
    int pid1 = fork();
    if(pid1<0)
    {
        perror("fork1 error\n");
        return -1;
    }
    if(pid1==0)
    {
        close(pipefd[0]);//关闭读端
        dup2(pipefd[1],1);
        execlp("ps","ps","-ef",NULL);
        exit(0);
    }
    int pid2 = fork();
    if(pid2<0)
    {
        perror("fork2 error\n");
        return -1;
    }
    if(pid2==0)
    {
        close(pipefd[1]);//关闭写端
        dup2(pipefd[0],0);//把从标准输入获取数据改成从匿名管道获取
        execlp("grep","grep","ssh",NULL);
        exit(0);
    }
    close(pipefd[0]);
    close(pipefd[1]);
    waitpid(pid1,NULL,0);
    waitpid(pid2,NULL,0);
    return 0;
}

