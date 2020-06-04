#include <iostream>
#include <arpa/inet.h>
using namespace std;
int main()
{
    char arr[]="192.169.12.0";
    char buf[32];
    inet_pton(AF_INET,arr,buf);
    for(int i=0;i<32;i++)
        cout<<buf[i]+'0';
    return 0;
}

