#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
//这个程序用来了解exec函数族的使用

int main()
{
    execl("/bin/ls","ls","-l","-a",NULL);
    return 0;
}

