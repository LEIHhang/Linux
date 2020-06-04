/*这个demo用于修改发送缓冲区大小，向指定的地址不断发送
 * 直到将所有数据发送成功*/
#include <iostream>
#include <arpa/inet.h>
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#define BUFFER_SIZE 512

int main(int argc,char* argv[])
{
    if(argc <= 2)
    {
        printf("input error\n");
        return -1;
    }
    const char* ip = argv[1];
    int port = atoi(argv[2]);

    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    inet_pton(AF_INET,ip,&server_address.sin_addr);
    server_address.sin_port = htons(port);

    int sock = socket(AF_INET,SOCK_STREAM,0);
    assert(sock >= 0);

    int sendbuf = atoi(argv[3]);
    int len = sizeof(sendbuf);
    //设置发送缓冲区大小
    setsockopt(sock,SOL_SOCKET,SO_SNDBUF,&sendbuf,sizeof(sendbuf));
    getsockopt(sock,SOL_SOCKET,SO_SNDBUF,&sendbuf,(socklen_t*)&len);
    printf("the tcp send buffer after setting is :%d \n",sendbuf);

    if(connect(sock,(struct sockaddr*)&server_address,sizeof(server_address)) != -1)
    {
        char buffer[ BUFFER_SIZE ];
        memset( buffer, 'a', BUFFER_SIZE );
        send(sock,buffer,BUFFER_SIZE,0);
    }
    close(sock);

    return 0;
}

