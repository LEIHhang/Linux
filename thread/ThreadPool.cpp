#include <iostream>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <queue>
using namespace std;
//typedef一个函数指针
typedef void (*TaskHandler)(int data);

class ThreadTask
{
    private:
        int _data;
        TaskHandler _handler;
    public:
        ThreadTask(int data,TaskHandler handler):_data(data),_handler(handler)
    {}
        ThreadTask(){}
        bool TaskSet(int data,TaskHandler handler)
        {
            _data = data;
            _handler = handler;
            return true;
        }
        bool TaskRun()
        {
            _handler(_data);
            return true;
        }
};

#define MAX_THR 4
class ThreadPool
{
    private:
        std::queue<ThreadTask> task_queue;
        int thr_num;
        pthread_mutex_t _mutex;
        pthread_cond_t _cond_con;
        pthread_cond_t _cond_pro;
    public:
        void ThreadLock()
        {
            pthread_mutex_lock(&_mutex);
        }
        void ThreadUnlock()
        {
            pthread_mutex_unlock(&_mutex);
        }

        bool QueueEmpty()
        {
            return task_queue.empty();
        }
        void ThreadWait()
        {
            pthread_cond_wait(&_cond_con,&_mutex);
        }
        bool QueuePop(ThreadTask &tt)
        {
            tt = task_queue.front();
            task_queue.pop();
            return true;
        }

    private:
        //必须设置为静态函数,因为如果是成员函数，参数中有this指针，不符合接口
        static void *thr_start(void *arg)
        {//通过传递this指针才能获取当前线程池中的任务队列中的任务。
            ThreadPool* pool = (ThreadPool*)arg;
            while(1)
            {
                pool->ThreadLock();
                while(pool->QueueEmpty())
                {
                    pool->ThreadWait();
                }
                ThreadTask tt;
                pool->QueuePop(tt);
                pool->ThreadUnlock();
                tt.TaskRun();
            }
            return NULL;
        }
    public:
        ThreadPool(int max = MAX_THR):thr_num(max)
    {}
        //设置初始化接口的原因是，构造函数无法获取返回值，若创建线程失败我们则无从得知。
        //1.创建线程
        //2.初始化互斥锁和条件变量
        bool PoolInit()
        {
            pthread_t tid[MAX_THR];
            pthread_mutex_init(&_mutex,NULL);
            pthread_cond_init(&_cond_con,NULL);
            pthread_cond_init(&_cond_pro,NULL);
            for(int i = 0; i<thr_num;i++)
            {
                int ret = pthread_create(&tid[i],NULL,thr_start,(void*)this);
                if(ret!=0)
                {
                    printf("create error\n");
                    return false;
                }
                pthread_detach(tid[i]);
            }
            return true;
        }
        //设置插入接口的目的是为了让用户插入所需要处理的代码
        bool TaskPush(ThreadTask &tt)
        {
            pthread_mutex_lock(&_mutex);
            task_queue.push(tt);
            pthread_mutex_unlock(&_mutex);
            pthread_cond_signal(&_cond_con);
            return true;
        }
};
    void test(int data)
    {
            printf("this is %p --get data:%d \n",pthread_self(),data);
                sleep(1);//睡眠一秒，方便查看
                
    }
int main()
{
    ThreadPool pool;
    pool.PoolInit();
    for(int i = 0;i<10 ;i++)
    {
        ThreadTask task(i,test);
        pool.TaskPush(task);
    }
    while(1)
    {
        sleep(1);
    }
    return 0;
}

