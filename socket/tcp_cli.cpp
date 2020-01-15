#include <iostream>
#include "TcpSocket.hpp"
int main(int argc,char *argv[])
{
    if(argc != 3)
    {
        std::cout<<"./tcp_cli ip port\n";
        return -1;
    }
    
    std::string ip = argv[1];
    uint16_t port = atoi(argv[2]);

    TcpSocket cli_sock;
    CHECK_RET(cli_sock.Socket());

    CHECK_RET(cli_sock.Connect(ip, port));
    while(1)
    {
        std::string buf;
        std::cout<< "client say:";
        fflush(stdout);
        cin>>buf;
        bool ret = cli_sock.Send(buf);
        if(ret==false)
        {
            cli_sock.Close();
            return -1;
        }
        buf.clear();
        cli_sock.Recv(buf);
        std::cout << "server say:" << buf << std::endl;
    }
    cli_sock.Close();
    return 0;
}

