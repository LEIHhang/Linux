/*************************************************************************
*File Name: sendfile_.cpp
*Author: Leihang
*mail: 973331055@qq.com
*Created Time: 2020年06月05日 星期五 17时01分00秒
*describe:这个demo用于使用sendfile函数，使用sendfile函数将服务器上
的一个文件传送给客户端
************************************************************************/
#include <iostream>
#include <netinet/in.h> //定义struct sockaddr_in
#include <arpa/inet.h> //定义点分十进制到网络字节序IP地址的转换等转换函数
#include <assert.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>//atoi
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/sendfile.h>
#include <sys/stat.h>
#include <unistd.h>

int main(int argc,char* argv[])
{
    if(argc<=3)
    {
        printf("please ip_address port_number filename\n");
        return 1;
    }
    const char* ip = argv[1];
    int port = atoi(argv[2]);
    const char* file_name = argv[3];

    int filefd=open(file_name,O_RDONLY);
    assert(filefd>0);
    struct stat stat_buf;
    fstat(filefd,&stat_buf);

    struct sockaddr_in address;
    bzero(&address,sizeof(address));
    address.sin_family = AF_INET;
    inet_pton(AF_INET,ip,&address.sin_addr);
    address.sin_port = htons(port);

    int sock = socket(AF_INET,SOCK_STREAM,0);
    assert(sock>=0);

    int ret = bind(sock,(struct sockaddr*)&address,sizeof(address));
    assert(ret!=-1);

    ret = listen(sock,5);
    assert(ret!=-1);

    struct sockaddr_in client;
    socklen_t client_addrlenth = sizeof(client);
    int connfd = accept(sock,(struct sockaddr*)&client,&client_addrlenth);
    if(connfd < 0)
    {
        perror("accept error\n");
    }
    else
    {
        //sendfile(int out_fd,int in_fd,off_t* offset,size_t count)
        //从in_fd文件描述中，从offset位置开始读取count个数据存入out_fd套接字描述符中
        //返回值是成功写入的数据字节数，失败返回-1并设置errno
        sendfile(connfd,filefd,NULL,stat_buf.st_size);
        close(connfd);
    }
    close(sock);
    return 0;
}

