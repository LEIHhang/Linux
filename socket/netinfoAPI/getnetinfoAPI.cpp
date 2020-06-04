/*************************************************************************
*File Name: getserentby.cpp
*Author: Leihang
*mail: 973331055@qq.com
*Created Time: 2020年06月04日 星期四 22时43分46秒
*describe:这个demo用来学习了解getservbyname和getservbyport
这两个网络信息获取接口,通过主机名来访问目标服务器上的daytime服务，
以获取该机器的系统时间
************************************************************************/
#include <iostream>
#include <sys/socket.h>
#include <netdb.h>  //网络信息API都在这个头文件里
#include <netinet/in.h> //socketaddr_in结构体定义位置
#include <stdio.h>
#include <unistd.h> //close定义头文件
#include <assert.h>
#include <errno.h>
#include <string.h>
#include <arpa/inet.h>
int main(int argc,char *argv[])
{
    assert( argc == 2 );
    char *host = argv[1];
    //获取目标主机地址信息
    struct hostent* hostinfo = gethostbyname( host );
    assert( hostinfo );
    //获取daytime服务信息
    struct servent* servinfo = getservbyname("daytime","tcp");
    assert(servinfo);
    printf("daytime port is %d\n",ntohs(servinfo->s_port));

    struct sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_port = servinfo->s_port;

    //注意下面的代码，因为h_addr_list本身是使用网络字节序的地址列表
    //所以使用其中的IP地址时，无需对目标IP地址转换字节序
    printf("address_ip is %d\n",hostinfo->h_addr_list);
    address.sin_addr = *(struct in_addr* )*hostinfo->h_addr_list;
    printf("address_ip is %d\n",address.sin_addr);
    printf("%s\n",inet_ntoa(address.sin_addr));


    int sockfd = socket(AF_INET,SOCK_STREAM,0);
    int result = connect(sockfd,(struct sockaddr*)&address,sizeof(address));
    printf("errno is %d\n",errno);
    assert(result!=-1);

    char buffer[128];
    result = read( sockfd,buffer,sizeof(buffer) );
    assert(result > 0);
    buffer[result]='\0';
    printf("the day time is:%s",buffer);
    close(sockfd);
    
    return 0;
}

