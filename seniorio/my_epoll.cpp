#include <iostream>
#include <sys/epoll.h>
#include "TcpSocket.hpp"
int main()
{
    TcpSocket listen_sock;
    CHECK_RET(listen_sock.Socket());
    CHECK_RET(listen_sock.Bind("0.0.0.0",8500));
    CHECK_RET(listen_sock.Listen());
    while(1)
    {

    }
    return 0;
}

