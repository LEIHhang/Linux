#include <stdio.h>
#include <unistd.h>
#include <signal.h>
int main()
{
    kill(getpid(),SIGQUIT);
    return 0;
}

