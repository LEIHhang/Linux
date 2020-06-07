/*************************************************************************
*File Name: tee_.cpp
*Author: Leihang
*mail: 973331055@qq.com
*Created Time: 2020年06月06日 星期六 16时22分41秒
*describe: 这个demo使用splice和tee函数实现Linux下的tee程序
（同时输出数据到终端和文件的程序，不要和tee函数混淆）的基本功能
************************************************************************/
#include <iostream>
#include <assert.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
int main(int argc,char* argv[])
{
    if(argc != 2)
    {
        printf("input error,input filename\n");
        return 1;
    }
    char* file_name = argv[1];
    int file_fd = open(file_name,O_WRONLY|O_CREAT|O_TRUNC,0666);
    assert(file_fd>0);

    int pipefd_sudout[2];
    int ret = pipe(pipefd_sudout);
    assert(ret!=-1);

    int pipefd_file[2];
    ret = pipe(pipefd_file);
    assert(ret!=-1);

    //将标准输入内容放入stdout管道内。
    ret = splice(STDIN_FILENO,NULL,pipefd_sudout[1],NULL,32768
                 ,SPLICE_F_MORE|SPLICE_F_MOVE);
    assert(ret!=-1);
    //将stdout管道内的数据复制到stdfile管道内
    ret = tee(pipefd_sudout[0],pipefd_file[1],32768,SPLICE_F_NONBLOCK);
    assert(ret != -1);
    //将stdout管道内的数据发送到标准输出
    ret = splice(pipefd_sudout[0],NULL,STDOUT_FILENO,NULL,32768
                 ,SPLICE_F_MORE|SPLICE_F_MOVE);
    assert(ret != -1);
    //将stdfile管道内的数据发送到文件
    ret = splice(pipefd_file[0],NULL,file_fd,NULL,32768
                 ,SPLICE_F_MORE|SPLICE_F_MOVE);
    assert(ret != -1);

    close(file_fd);
    close(pipefd_sudout[0]);
    close(pipefd_sudout[1]);
    close(pipefd_file[0]);
    close(pipefd_file[1]);
    return 0;
}

