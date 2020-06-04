#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
int main()
{
    pid_t pid;
    if(pid=fork()<0)
        printf("fork error\n");
    else if(pid==0)//子程序
    {
        if(execl("/home/liangzai/Linux/my_pra/process/explain.log","explain.log","arg1","arg2",(char*)0)<0)
            printf("execl error,%d\n",getpid());
    }
    if(wait(NULL)<0)
        printf("wait error\n");
    return 0;
}

