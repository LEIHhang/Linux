#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <pthread.h>
using namespace std;
//1.创建三个线程，让一个线程获取锁后进入无限循环，查看另外两个线程状态
pthread_mutex_t _mutex;
void *thr_start(void *arg)
{
    pthread_mutex_lock(&_mutex);
    int i=0;
    while(1)
    {
        cout<<i++<<endl;
    }
    pthread_mutex_unlock(&_mutex);
}
#define MAX_THR 3
int main()
{
    pthread_t tid[3];
    pthread_mutex_init(&_mutex,NULL);
    for(int i=0;i<MAX_THR;i++)
    {

        int ret = pthread_create(&tid[i],NULL,thr_start,NULL);
        if(ret!=0)
        {
            cout << "create error"<<endl;
            return -1;
        }
    }
    pthread_mutex_destroy(&_mutex);
    for(int i=0;i<MAX_THR;i++)
    {
        pthread_join(tid[i],NULL);
    }
    return 0;
}

