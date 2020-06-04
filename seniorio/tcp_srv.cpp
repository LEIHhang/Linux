#include <iostream>
#include <string>
#include <stdlib.h>
#include "TcpSocket.hpp"
#include "select.hpp"
int main(int argc,char* argv[])
{
    if(argc!=3)
    {
        cout<<"input error\n";
        return 0;
    }
    TcpSocket lst_sock;
    CHECK_RET(lst_sock.Socket());
    CHECK_RET(lst_sock.Bind(argv[1],atoi(argv[2])));
    CHECK_RET(lst_sock.Listen());

    Select s;
    s.Add(lst_sock);
    while(1)
    {
        std::vector<TcpSocket> list;
        bool ret = s.Wait(&list);
        if(ret == false)
            continue;
        for(auto sock:list)
        {
            if(sock.Getfd()==lst_sock.Getfd())
            {
                TcpSocket cli_sock;
                bool ret = sock.Accept(&cli_sock);
                if(ret == false)
                    continue;
                s.Add(cli_sock);
                std::cout<<"add a socket\n";
            }
            else
            {
                std::string buf;
                ret = sock.Recv(buf);
                if(ret == false)
                {
                    //将不在打开的描述符从监控描述符集合中去除
                    s.Del(sock);
                    sock.Close();
                    continue;
                }
                std::cout<<"cli say:"<<buf<<endl;
                buf.clear();
                std::cout<<"server say:";
                std::cin>>buf;
                ret = sock.Send(buf);
                if(ret == false)
                {
                    s.Del(sock);
                    sock.Close();
                    continue;
                }
                
            }
        }

    }
    

    return 0;
}

