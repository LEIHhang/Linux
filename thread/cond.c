//通过吃面的demo体会临界资源访问的时序合理性以及条件变量的基本使用
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
int is_have_noodle = 0;
pthread_cond_t cond_foodie;
pthread_cond_t cond_chef;
pthread_mutex_t mutex;
void *foodie(void *arg)
{
    while(1)
    {
        pthread_mutex_lock(&mutex);
        //使用while的原因：当有多个吃面的人，如果现在没有资源可以使用，则多个吃面的人都处在等待队列上
        //若此时唤醒线程，可能会全部唤醒，但是资源只有一份，当一个人抢到锁了后，其他的人就被锁阻止了，直到第一个
        //抢到锁的人解锁后其他人进行加锁，但此时必须要再判断，否则会误以为还有面可以吃。再次进入等待队列。
        while(is_have_noodle==0)
        {
           // pthread_mutex_unlock(&mutex);//若没有面，则对面的访问解锁，让厨师能获取到
           // pause();//让线程进行等待，等待其他线程唤醒他
           // pthread_mutex_lock(&mutex);//唤醒线程，对noodle进行操作
            //但是因为这三步操作不是原子性的，在实际运行过程中，可能会发生进程还没有等待，其他线程就对他进行唤醒，导致
            //线程始终睡眠，一直被阻塞。
            //因此，才有了pthread_cond_wait这个函数，它将这三步操作合为一体，成为原子性的。

            pthread_cond_wait(&cond_foodie,&mutex);//因为条件判断需要用户自己设置，要对临界资源进行访问，所以需要搭配互斥锁使用
            //没有面条，等待
            
            //不同的角色要等待在不同的等待队列上，因为：如果目前没有面，则所有的顾客都在等待队列上。
            //当厨师做好一碗面之后，因为while循环，也进入到了相同的等待队列上
            //然后进行唤醒，唤醒一个顾客吃面。当吃完面后再唤醒等待队列上的线程时，可能不是唤醒厨师，唤醒下一个顾客
            //然而下一个顾客通过条件判断进入到了等待队列上，并不会进行唤醒操作。所以程序就阻塞了。
        }
        printf("eat noodles, delicious~~~\n");
        is_have_noodle--;
        pthread_mutex_unlock(&mutex);
        pthread_cond_signal(&cond_chef);
    }
    return NULL;
}
void *chef(void *arg)
{
    while(1)
    {
        pthread_mutex_lock(&mutex);
        while(is_have_noodle==1)
        {
            pthread_cond_wait(&cond_chef,&mutex);
        }
        printf("cook noodles~~~\n");
        is_have_noodle++;
        pthread_mutex_unlock(&mutex);
        pthread_cond_signal(&cond_foodie);
    }
    return NULL;
}
#define MAX_FOODIE 4
int main()
{
    pthread_t chef_tid;
    pthread_t foodie_tid[MAX_FOODIE];
    int ret=0;

    pthread_mutex_init(&mutex,NULL);
    pthread_cond_init(&cond_foodie,NULL);
    pthread_cond_init(&cond_chef,NULL);
    for(int i=0;i<MAX_FOODIE;i++)
    {
        ret = pthread_create(&foodie_tid[i],NULL,foodie,NULL);
        if(ret!=0)
        {
            printf("thread create error\n");
            return -1;
        }
    }
    ret = pthread_create(&chef_tid,NULL,chef,NULL);
    if(ret!=0)
    {
        printf("thread create error\n");
        return -1;
    }

    for(int i = 0;i < MAX_FOODIE;i++)
    {
        pthread_join(foodie_tid[i],NULL);
    }
    pthread_join(chef_tid,NULL);
    pthread_cond_destroy(&cond_foodie);
    pthread_cond_destroy(&cond_chef);
    pthread_mutex_destroy(&mutex);
    return 0;
}

