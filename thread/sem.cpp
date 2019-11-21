#include <iostream>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
sem_t sem;
void *thr_eat(void *arg)
{
    while(1)
    {
        sem_wait(&sem);
        //若资源<=0，则进行等待，并将资源数-1
        //若资源>0，则运行下去，并将资源数-1
        printf("delicious~~\n");
    }
    return NULL;
}
void *thr_cook(void *arg)
{
    while(1)
    {
        printf("cook noodles~~\n");
        sleep(1);
        sem_post(&sem);
        //唤醒等待队列,资源数+1==》
        //如果此时有等待线程，那么+1的意思是少了一个等待的线程
        //如果没有等待线程，那么+1意思是多了一个可以使用的资源
    }
    return NULL;
}
int main()
{
    int ret;
    sem_init(&sem,0,0);
    pthread_t tid1,tid2;
    ret = pthread_create(&tid1,NULL,thr_eat,NULL);
    if(ret != 0)
    {
        printf("create error\n");
        return -1;
    }
    ret = pthread_create(&tid1,NULL,thr_cook,NULL);
    if(ret != 0)
    {
        printf("create error\n");
        return -1;
    }
    pthread_join(tid1,NULL);
    pthread_join(tid2,NULL);
    sem_destroy(&sem);
    return 0;
}

