#include <iostream>
#include "TcpSocket.hpp"

int main(int argc ,char *argv[])
{
    if(argc != 3)
    {
        std::cout << "./tcp_srv ip port\n";
        return -1;
    }
    std::string ip = argv[1];
    uint16_t port = atoi(argv[2]);

    TcpSocket lst_sock;
    CHECK_RET(lst_sock.Socket());
    CHECK_RET(lst_sock.Bind(ip,port));
    CHECK_RET(lst_sock.Listen());
    while(1)
    {
        TcpSocket cli_socket;
        std::string cli_ip;
        uint16_t cli_port;
        if(lst_sock.Accept(&cli_socket, &cli_ip, &cli_port)== false)
        {
            //lst_sock.Close();
            continue;
        }
        std::cout<<"new connect:"<<cli_ip<<":"<<cli_port<<endl;
        pid_t pid = fork();
        if(pid == -1)
        {
            perror("fork error\n");
            return -1;
        }
        if(pid == 0)
        {
            while(1)
            {
                string buf;
                if(cli_socket.Recv(buf)==false)
                {
                    cli_socket.Close();
                    exit(0);
                }
                cout<<"client_say:"<<buf<<endl;
                cout<<"server say:";
                fflush(stdout);
                buf.clear();
                std::cin>>buf;

                if(cli_socket.Send(buf)==false)
                {
                    cli_socket.Close();
                    exit(0);
                }
            }
            cli_socket.Close();
        }
        cli_socket.Close();
    }
    return 0;
}

