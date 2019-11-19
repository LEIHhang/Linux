#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>

void *thr_start(void *arg)
{
    while(1){
        sleep(1);
    }
    return NULL;
}
int main()
{
    pthread_t tid;
    char *ptr = "this is thread\n";
    int ret=pthread_create(&tid,NULL,thr_start,(void*)ptr);//tid是返回型参数
    if(ret!= 0)
    {
        perror("thread creat error");
        return -1;
    }
    return 0;
}

