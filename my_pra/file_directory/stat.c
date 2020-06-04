#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
int main()
{
    struct stat* buf =(struct stat*) malloc(sizeof(struct stat));
    stat("test.log",buf);
    printf("%d\n",buf->st_uid);
    chown("test.log",1000,1000);
    return 0;
}

