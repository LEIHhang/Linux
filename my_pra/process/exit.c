#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
//通过对exit和atexie学习，atexit用来设置终止处理程序
//用来在进程结束的时候执行
//执行atexit最多可设置32个函数，并且后添加的程序先执行
//exit用来显示终止程序，参数是程数终止状态码
//调用主函数可以理解exit(main(argc,argc[]));
//所以使用return返回值就是终止状态码
//exit就是一个启动例程
static void exit_1();
static void exit_2();
int main()
{
    if(atexit(exit_1)!=0)
        printf("atexit error\n");
    if(atexit(exit_2)!=0)
        printf("atexit error\n");
    printf("main is done\n");
    return 0;
}
static void exit_1()
{
    printf("firstr exit handler\n");
}
static void exit_2()
{
    printf("second exit handler\n");
}
