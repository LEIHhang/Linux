/*************************************************************************
*File Name: splice_.cpp
*Author: Leihang
*mail: 973331055@qq.com
*Created Time: 2020年06月06日 星期六 15时01分18秒
*describe:这个demo用splice实现一个零拷贝的回射服务器 
************************************************************************/
#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>//sockaddr结构体
#include <arpa/inet.h> //将IP地址进行转换
#include <assert.h>
#include <stdio.h>
#include <unistd.h> //close函数
#include <stdlib.h>//atoi 
#include <errno.h>
#include <string.h>
#include <fcntl.h>

#define BACKLOG 5
#define handle_error(msg) \
    do { perror(msg); return 1; } while(0)
int main(int argc,char* argv[])
{
    if(argc <= 2)
    {
        printf("input error\n");       
    }
    char* ip = argv[1];
    int port = atoi(argv[2]);
    
    struct sockaddr_in address;
    address.sin_family = AF_INET;
    inet_pton(AF_INET,ip,&address.sin_addr);
    address.sin_port = htons(port);

    int sock = socket(AF_INET,SOCK_STREAM,0);
    if(sock<=0)
    {
        handle_error("sock\n");
    }
    int ret = bind(sock,(struct sockaddr*)&address,sizeof(address));
    if(ret = -1)
    {
        handle_error("bind\n");
    }
    ret = listen(sock,BACKLOG);
    if(ret = -1)
    {
        handle_error("listen\n");
    }

    struct sockaddr_in client_addr;
    int client_addrlength = sizeof(client_addr);
    int connfd = accept(sock,(struct sockaddr*)&client_addr,(socklen_t*)&client_addrlength);
    if(connfd<=0)
    {
        handle_error("accept\n");
    }

    int pipefd[2];
    ret = pipe(pipefd);
    ret = splice(connfd,NULL,pipefd[1],NULL,32768,SPLICE_F_MORE|SPLICE_F_MOVE);
    if(ret == -1)
    {
        handle_error("splice\n");
    }
    ret = splice(pipefd[0],NULL,connfd,NULL,32768,SPLICE_F_MORE|SPLICE_F_MOVE);
    if(ret == -1)
    {
        handle_error("splice\n");
    }
    close(connfd);

    close(sock);
    return 0;
}

