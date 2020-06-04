#include <iostream>
#include <unistd.h>
#include <stdio.h>
int glob=6;
char buf[]="a write to stdout\n";

int main()
{
    int var;
    pid_t pid;
    var = 88;
    if(write(STDOUT_FILENO,buf,sizeof(buf)-1)!=sizeof(buf)-1)
        std::cerr<<"write error\n";
    printf("before fork\n");

    if(pid=fork()<0)
    {
        std::cerr<<"fork error\n";
    }
    else if(pid==0)
    {
        var++;
        glob++;
    }
    else
    {
        sleep(2);
    }
    printf("pid=%d,glob=%d,var=%d\n",getpid(),glob,var);
    return 0;
}

