//通过黄牛抢票的例子体会线程安全的重要性以及认识互斥锁
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#define MAX_THR 4
pthread_mutex_t mutex;
int tickets = 100;
void *cattle(void *arg)
{
    while(1)
    {
        pthread_mutex_lock(&mutex);//阻塞加锁
       // pthread_mutex_trylock(&mutex);//非阻塞加锁
        if(tickets>0)
        {
            printf("%pi get a tickets %d\n",pthread_self(),tickets);
            tickets--;
            pthread_mutex_unlock(&mutex);
            usleep(1000);
        }
        else
        {
            printf("have no tickets\n");
            //加锁之后需要在任意有可能退出线程的地方都需要进行解锁
            pthread_mutex_unlock(&mutex);
            pthread_exit(NULL);
        }
    }
    return NULL;
}
int main()
{
    pthread_t tid[MAX_THR];
    
    pthread_mutex_init(&mutex,NULL);

    for(int i = 0;i <MAX_THR; i++)
    {
        int ret = pthread_create(&tid[i],NULL,cattle,NULL);
        if(ret!=0)
        {
            printf("create error");
            return -1;
        }
    }
    for(int i=0;i<MAX_THR; i++)
    {
        pthread_join(tid[i],NULL);
    }
    pthread_mutex_destroy(&mutex);
    return 0;
}

