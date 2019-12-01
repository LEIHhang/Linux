//通过大妈抢鸡蛋的例子，体会线程安全的重要性以及认识了解互斥锁
//每个线程代表一个大妈。
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#define MAX_THR 4 //设置创建线程数量
pthread_mutex_t _mutex;//创建互斥锁变量
int egg = 100;//设置100个鸡蛋
void *Dama(void *arg)
{
    while(1)
    {
        pthread_mutex_lock(&_mutex);//先上锁，让其他线程阻塞    
        if(egg > 0)                                                 
        {
            printf("%p抢到了第%d个鸡蛋\n",pthread_self(),egg--);   
        }                                                           
        else                                                           
        {                                                           
            printf("鸡蛋抢完了\n");                                 
            pthread_mutex_unlock(&_mutex);//解锁后退出线程          
            pthread_exit(NULL);                                     
        }                                                           
        pthread_mutex_unlock(&_mutex);//抢到了进行解锁
        usleep(100);
    }
    return NULL;
}
int main()
{
    int i;
    pthread_t tid[MAX_THR];//线程ID

    pthread_mutex_init(&_mutex,NULL);//互斥锁初始化

    for(i = 0;i < MAX_THR; i++)
    {
        int ret = pthread_create(&tid[i],NULL,Dama,NULL);//创建线程
        //参数解释：
        //1.保存当前线程ID
        //2.设置线程属性
        //3.设置线程要去执行的函数地址
        //4.需要给线程函数中传递的参数
        if(ret!=0)//判断线程是否为创建失败
        {
            perror("pthread error\n");
            return -1;
        }
    }
    for(i = 0;i < MAX_THR; i++)
    {
        pthread_join(tid[i],NULL);//线程等待
    }
    pthread_mutex_destroy(&_mutex);//销毁互斥锁
    return 0;
}

