#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
int main()
{
    write(1,"这是第一条数据",22);
    printf("这是第二条数据");
    fwrite("这是第三条数据",22,1,stdout);
    
    sleep(3);
    return 0;
}

