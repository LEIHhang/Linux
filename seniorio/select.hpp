#pragma once
#include<sys/select.h>
#include<iostream>
#include<stdlib.h>
#include<vector>
#include"TcpSocket.hpp"

class Select
{
public:
    Select():maxfd(0) {
        FD_ZERO(&_rfdset);
    }
    ~Select() {}
    bool Add(TcpSocket &sock)
    {
        int fd = sock.Getfd();
        FD_SET(fd,&_rfdset);
        maxfd = maxfd > fd ? maxfd : fd;
        return true;
    }
    bool Del(TcpSocket &sock)
    {
        int fd = sock.Getfd();
        FD_CLR(fd,&_rfdset);
        for(int i = maxfd;i>=0;i--)
        {
            if(FD_ISSET(i,&_rfdset))
            {
                maxfd = i;
                break;
            }
        }
        return true;
    }
    bool Wait(std::vector<TcpSocket> *list,int timeout = 3)
    {
        struct timeval tv;
        tv.tv_sec = timeout;
        tv.tv_usec = 0;
        fd_set readfds = _rfdset;
        int ret = select(maxfd+1,&readfds,NULL,NULL,&tv);
        if(ret == 0)
        {
            printf("time out\n");
            return false;
        }
        else if(ret<0)
        {
            perror("select()\n");
            return false;
        }
        for(int i = 0; i<maxfd+1 ; i++)
        {
            if(FD_ISSET(i,&readfds))
            {
                TcpSocket sock;
                sock.Setfd(i);
                list->push_back(sock);
            }
        }
        return true;
        
    }
private:
    fd_set _rfdset;//原始监控集合
    int maxfd;
};

