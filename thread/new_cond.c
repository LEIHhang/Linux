//通过生产者和消费者，来体会对临界资源的访问时序性
//和对条件变量的使用
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
pthread_cond_t _con;
pthread_cond_t _pro;//定义生产者和消费者条件变量

pthread_mutex_t _mutex;//定义互斥锁

int is_hava_resource = 0;//初始化资源数量为0
void *thr_con(void *arg)
{
    while(1)
    {
        pthread_mutex_lock(&_mutex);
        while(is_hava_resource == 0)
        {
            pthread_cond_wait(&_con,&_mutex);//让消费者进行等待
        }
        is_hava_resource--;
        printf("i get a resource\n");
        pthread_mutex_unlock(&_mutex);
        pthread_cond_signal(&_pro);//唤醒生产者进行生产
    }
}
void *thr_pro(void *arg)
{
    while(1)
    {
        pthread_mutex_lock(&_mutex);
        while(is_hava_resource == 1)
        {
            pthread_cond_wait(&_pro,&_mutex);//让生产者进行等待
        }
        is_hava_resource++;
        printf("i make a resource\n");
        pthread_mutex_unlock(&_mutex);
        pthread_cond_signal(&_con);//唤醒消费者进行消耗
    }
}

int main()
{
    //1.分别创建一个生产者线程和消费者线程
    //2.分别设置线程执行函数，一个进行消耗资源，一个负责生产资源
    
    pthread_cond_init(&_con,NULL);
    pthread_cond_init(&_pro,NULL);
    pthread_mutex_init(&_mutex,NULL);
    pthread_t con_tid;
    pthread_t pro_tid;
    int ret;
    ret = pthread_create(&con_tid,NULL,thr_con,NULL);
    if(ret != 0)
    {
        perror("create error\n");
        return -1;
    }
    ret = pthread_create(&pro_tid,NULL,thr_pro,NULL);
    if(ret != 0)
    {
        perror("create error\n");
        return -1;
    }
    pthread_join(con_tid,NULL);
    pthread_join(pro_tid,NULL);
    pthread_cond_destroy(&_con);
    pthread_cond_destroy(&_pro);
    pthread_mutex_destroy(&_mutex);
    return 0;
}

