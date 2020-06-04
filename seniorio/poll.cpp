#include <iostream>
#include <sys/poll.h>
#include "TcpSocket.hpp"
#define MAXSIZE 1024
int main()
{
    TcpSocket lst_sock;
    CHECK_RET(lst_sock.Socket());
    CHECK_RET(lst_sock.Bind("0.0.0.0",9000));
    CHECK_RET(lst_sock.Listen());
    struct pollfd ap[MAXSIZE];
    ap[0].fd = lst_sock.Getfd();
    ap[0].events = POLLIN;
    int count = 1;
    while(1)
    {
        int num = poll(ap,MAXSIZE,-1);
        if(num<0)
        {
            perror("poll error\n");
            continue;
        }
        int number = count;
        for(int i = 0;i < number;i++)
        {
            if(ap[i].revents == POLLIN)
            {
                if(ap[i].fd == lst_sock.Getfd())
                {
                    TcpSocket cli_sock;
                    lst_sock.Accept(&cli_sock);
                    ap[count].fd = cli_sock.Getfd();
                    ap[count].events = POLLIN;
                    ++count;
                }
                else
                {
                    string buf;
                    TcpSocket cli_sock;
                    cli_sock.Setfd(ap[i].fd);
                    int ret = cli_sock.Recv(buf);
                    if(ret == 0)
                    {
                        cli_sock.Close();
                    }
                    cout<<"cli say:"<<buf<<endl;
                    fflush(stdout);
                    buf.clear();
                    cout<<"srv say:";
                    cin>>buf;
                    cli_sock.Send(buf);
                }
            }
        }
    }
    return 0;
}

