#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <vector>
#include <semaphore.h>
using namespace std;
#define max_queue 10


class BlockQueue
{
    private:
        vector<int> queue;
        int capacity;
        int _step_read;
        int _step_write;
        sem_t _lock;
        sem_t sem_con;
        sem_t sem_pro;
    public:
        BlockQueue(int max_q = max_queue):capacity(max_q),queue(max_q),_step_read(0),_step_write(0)
        {
            sem_init(&_lock,0,1);
            sem_init(&sem_con,0,0);
            sem_init(&sem_pro,0,max_q);
        }
        ~BlockQueue()
        {
            sem_destroy(&_lock);
            sem_destroy(&sem_con);
            sem_destroy(&sem_pro);
        }
        bool queue_push(int &data)
        {
            //1.使用信号量等待，判断是否有位置可插入数据
            sem_wait(&sem_pro);
            //2.先上锁
            //这里的锁目的是为了保护数据访问的，不能放在条件判断之前
            sem_wait(&_lock);
            //3.插入数据
            queue[_step_write]=data;
            _step_write = (_step_write +1 )% capacity;
            //4.唤醒消费者
            sem_post(&sem_con);
            //5.解锁
            sem_post(&_lock);
        }
        bool queue_pop(int &data)
        {
            //1.使用信号量等待，判断是否还有资源可以获取
            sem_wait(&sem_con);
            //2.上锁//这里的上锁目的是和相同线程间进行互斥
            //本来应该不同角色设置一把锁，如果是多核CPU就能实现并发
            sem_wait(&_lock);
            //3.进行获取数据操作
            data = queue[_step_read];
            _step_read = (_step_read + 1)% capacity;
            //4.唤醒生产者可以进行数据的插入了
            sem_post(&sem_pro);
            //5.解锁
            sem_post(&_lock);
        }
};
void *thr_con(void *arg)
{
    BlockQueue *queue = (BlockQueue*)arg;
    while(1)
    {
        int data;
        queue->queue_pop(data);
        printf("**get a data %d \n",data);
    }
    return NULL;
}
void *thr_pro(void *arg)
{
    BlockQueue *queue = (BlockQueue*)arg;
    int data = 0;
    while(1)
    {
        queue->queue_push(data);
        printf("--push a data %d \n",data++);
    }
    return NULL;
}
#define MAX_THR 4
int main()
{
    int ret;
    pthread_t ctid[MAX_THR],ptid[MAX_THR];
    
    BlockQueue _queue;
    for(int i=0; i<MAX_THR; i++)
    {
       ret= pthread_create(&ctid[i],NULL,thr_con,(void*)&_queue);
       if(ret!=0)
       {
           printf("create thread error\n");
           return -1;
       }
    }
    for(int i=0; i<MAX_THR; i++)
    {
       ret= pthread_create(&ptid[i],NULL,thr_pro,(void*)&_queue);
       if(ret!=0)
       {
           printf("create thread error\n");
           return -1;
       }
    }
    for(int i=0; i<MAX_THR; i++)
    {
        pthread_join(ctid[i],NULL);
        pthread_join(ptid[i],NULL);
    }
    return 0;
}

