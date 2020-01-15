#include <iostream>
#include <string>
#include <sstream>
#include "TcpSocket.hpp"

int main(int argc ,char *argv[])
{
    if(argc!=3)
    {
        std::cerr<<"./http ip port\n";
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
        TcpSocket cli_sock;
        if(lst_sock.Accept(&cli_sock)==false)
        {
            continue;
        }
        string buf;
        if(cli_sock.Recv(buf)==false)
        {
            cli_sock.Close();
            continue;
        }
        cout<<"req:["<<buf<<"]\n";
        string text;
        text = "<html><body><h1></h1>Change world</h1></body></html>";
        std::stringstream tmp;
        tmp << "HTTP/1.1 200 OK\r\n";
        tmp << "Content-Length: "<< text.size()<<"\r\n";
        tmp << "Connection: clost\r\n";
        tmp << "Content-Type: text/html\r\n";
        tmp << "\r\n";
        tmp << text;

        cli_sock.Send(tmp.str());
        cli_sock.Close();
    }
    lst_sock.Close();
    return 0;
}

