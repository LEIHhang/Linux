//这个demo用来学习select的使用，和一些编程注意事项
//对标准输入进行监控
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/select.h>
int main()
{
    fd_set read_fds;
    while(1)
    {
        //注意1.每次select后，集合都会改变，所以需要每次重新设置集合
        //2.每次select后，时间也会重置，所以轮询遍历的时候也需要重新设置
        FD_ZERO(&read_fds);
        FD_SET(0,&read_fds);
        printf(">\n");
        fflush(stdout);
        int maxfd=0;
        struct timeval tm;
        tm.tv_sec = 3;
        tm.tv_usec = 0;
        int ret = select(maxfd+1,&read_fds,NULL,NULL,&tm);
        if(ret==0)
        {
            printf("time out\n");
            continue;
        }
        else if(ret<0)
        {
            perror("select errot\n");
            exit(0);
        }
        for(int i = 0;i<=maxfd+1;i++)
        {
            if(FD_ISSET(i,&read_fds))
            {
                char buf[1024];
                read(i,buf,1024);
                printf("%s\n",buf);
            }
        }
    }
    return 0;
}

