//防止多次包含
#ifndef __M_TCP_H
#define __M_TCP_H

#pragma once
#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>
#include<error.h>
#include<iostream>
#include<string>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
using namespace std;
#define MAX_LISTEN 10
#define MAX_RECV 4096
#define CHECK_RET(q) if((q)==false){return -1;}
class TcpSocket
{
public:
    TcpSocket():_socket(-1) {}
    ~TcpSocket() {}
public:
    int Getfd()
    {
        return _socket;
    }
    void Setfd(int fd)
    {
        _socket = fd;
    }
    //1.创建套接字
    bool Socket()
    {
        _socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        if(_socket <= 0)
        {
            perror("socket error\n");
            return false;
        }
        return true;
    }
    //2.绑定地址信息
    bool Bind(string ip,uint16_t port)
    {
        //创建IPV4地址信息结构体
        struct sockaddr_in addr;
        //设置为IPV4信息结构体
        addr.sin_family = AF_INET;
        //将主机字节序转换成网络字节序，设置信息结构体端口号
        addr.sin_port = htons(port);
        //inet_addr将点分十进制的字符串转换为二进制数据字节序，设置信息结构体IP地址
        addr.sin_addr.s_addr = inet_addr(ip.c_str());

        socklen_t len = sizeof(struct sockaddr_in);
        //参数设置的时候进行强制类型转换
        int ret = bind(_socket , (struct sockaddr*)&addr , len);
        if(ret != 0)
        {
            perror("bind error\n");
            return false;
        }
        return true;
    }
    //3.开始监听（服务端）
    bool Listen(int backlog = MAX_LISTEN)
    {
        int ret = listen(_socket, backlog);
        //listen用的是服务端的socketid
        if(ret != 0)
        {
            perror("listen error\n");
            return false;
        }
        return true;
    }
    //3.建立连接（客户端）
    bool Connect(const string &ip,const uint16_t port)
    {
        struct sockaddr_in addr;

        addr.sin_family = AF_INET;
        addr.sin_port = htons(port);
        addr.sin_addr.s_addr = inet_addr(ip.c_str());
        socklen_t len = sizeof(addr);

        int ret = connect(_socket , (struct sockaddr*)&addr ,len );
        if(ret != 0)
        {
            perror("connect error\n");
            return false;
        }
        return true;
    }
    //4.获取新连接（服务端）
    bool Accept(TcpSocket *sock, string *ip = NULL, u_int16_t *port = NULL)
    {
        struct sockaddr_in addr;
        socklen_t len = sizeof(addr);
        int ret = accept(_socket , (struct sockaddr*)&addr, &len); 
        //addr能获取对方的Ip port信息
        if(ret < 0)
        {
            perror("accept error\n");
            return false;
        }
        sock->_socket = ret;
        if(ip!=NULL)
        {
            *ip = inet_ntoa(addr.sin_addr);
        }
        if(port != NULL)
        {
            *port = ntohs(addr.sin_port);
        }
        return true;
    }
    //5.发送数据
    bool Send(const string &buf)
    {
       int ret = send(_socket , &buf[0] ,buf.size(), 0);
       if(ret < 0)
       {
           perror("send error\n");
           return false;
       }
       return true;
    }
    //6.接收数据
    bool Recv(string &buf)
    {
        char tmp[MAX_RECV] = {0};
        int rlen = recv(_socket , tmp , MAX_RECV, 0);
        if(rlen < 0)
        {
            perror("recv error\n");
            return false;
        }
        else if(rlen == 0)
        {
            std::cerr<<"peer shutdown\n";
            return false;
        }
        buf.assign(tmp,rlen);
        return true;
    }
    //7.关闭套接字
    bool Close()
    {
        close(_socket);
        return true;
    }

private:
    int _socket;
};
#endif
