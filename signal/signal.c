#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
void sigcb(int signum)
{
    printf("recv a signal:%d\n",signum);
}
int main()
{
    //signal(2,SIG_IGN);//忽略
    signal(3,sigcb);//自定义
    //int sigaction(int signum, const struct sigaction *act, struct sigaction *oldact);
    //signum:信号值
    //act:signum当前要修改的新的动作
    //oldact:用于获取signum信号原有的动作（便于以后再还原回去）
    struct sigaction newact;
    struct sigaction oldact;
    //newact.sa_handler = sigcb;//设置自定义回调函数
    //newact.sa_flags = 0;//默认使用sa_handler回调函数
    //newact.sa_mask;//临时要阻塞的信号集合
    //sigemptyset(&newact.sa_mask);//清空

    //sigaction(2,&newact,&oldact);
    while(1){
        printf("-------\n");
        sleep(10);
    }
    return 0;
}

