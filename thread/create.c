#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
void *thr_start(void *arg)//返回值类型为void*，不要错误认为是函数指针
{
   // sleep(5);
   // pthread_exit(NULL);//线程退出，谁调用谁退出
   pthread_detach(pthread_self());
   char *ptr = "aiwozhonghau~\n";
   pthread_exit(ptr);
    while(1)
    {
        printf("child thread---%s\n",(char*)arg);
        sleep(1);
    }
    return NULL;
}
int main()
{
    pthread_t tid;
    char *ptr="this is a thread\n";
    int ret = pthread_create(&tid,NULL,thr_start,(void*)ptr);
    if(ret!=0)
    {
        perror("thread create error");
        return -1;
    }
    // sleep(5);
    // pthread_cancel(tid);//调用线程退出
    //将主线程进程终止，就会导致所有线程变成僵尸状态
    sleep(5);
    char *retval;
    pthread_join(tid,(void**)&retval);//获取char *类型数据，所以要这样定义
    printf("pthread_join:%s\n",retval);
    while(1)
    {
        printf("main thread---%d\n",tid);
        sleep(1);
    }
    return 0;

}

