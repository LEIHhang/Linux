#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <queue>
using namespace std;

#define MAX_THR 4
class BlockQueue
{
    private:
        std::queue<int> _queue;//设置动态增长队列
        int _capacity;//确定队列的最大容量
        pthread_cond_t _cond_consumer;//消费者等待队列
        pthread_cond_t _cond_productor;//生产者等待队列
        pthread_mutex_t _mutex;//设置互斥锁
    public:
        BlockQueue(int max_queue = 10):_capacity(max_queue) 
        {
            pthread_cond_init(&_cond_consumer,NULL);
            pthread_cond_init(&_cond_productor,NULL);
            pthread_mutex_init(&_mutex,NULL);
        }
        ~BlockQueue()
        {
            pthread_cond_destroy(&_cond_consumer);
            pthread_cond_destroy(&_cond_productor);
            pthread_mutex_destroy(&_mutex);
        }
        //提供给生产者的接口，数据入队
        bool queue_push(int &data)
        {
            pthread_mutex_lock(&_mutex);
            while(_queue.size() == _capacity)//判断队列节点是否添加满了
            {
                pthread_cond_wait(&_cond_productor,&_mutex);
            }
            _queue.push(data);
            pthread_mutex_unlock(&_mutex);
            pthread_cond_signal(&_cond_consumer);
            return true;
        }
        //提供给消费者的接口————数据出口
        bool queue_pop(int &data)
        {
            pthread_mutex_lock(&_mutex);
            while(_queue.empty())
            {
                pthread_cond_wait(&_cond_consumer,&_mutex);
            }
            data=_queue.front();
            _queue.pop();
            pthread_mutex_unlock(&_mutex);
            pthread_cond_signal(&_cond_productor);
            return true;
        }

};
void *thr_consumer(void *arg)
{
    BlockQueue* b=(BlockQueue*)arg;
    while(1)
    {
        int data;
        b->queue_pop(data);
        printf("i get a data %d\n",data);
    }
    return NULL;
}
void *thr_productor(void *arg)
{
    int data = 0;
    BlockQueue* b=(BlockQueue*)arg;
    while(1)
    {
        b->queue_push(data);
        printf("i put a data %d\n",data++);
    }
    return NULL;
}
int main()
{
    pthread_t ctid[MAX_THR],ptid[MAX_THR];
    BlockQueue queue;
    for(int i=0;i<MAX_THR;i++)
    {
        int ret=pthread_create(&ctid[i],NULL,thr_consumer,(void*)&queue);
        if(ret!=0)
        {
            printf("create thr error\n");
            return -1;
        }
    }
    for(int i=0;i<MAX_THR;i++)
    {
        int ret=pthread_create(&ctid[i],NULL,thr_productor,(void*)&queue);
        if(ret!=0)
        {
            printf("create thr error\n");
            return -1;
        }
    }
    for(int i=0;i<MAX_THR;i++)
    {
        pthread_join(ptid[i],NULL);
        pthread_join(ctid[i],NULL);
    }
    return 0;
}

