/*标准库IO接口回顾*/
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
int main()
{
    FILE *fp=NULL;
    fp=fopen("./tmp.tex","a+");
    if(fp==NULL)
    {
        perror("fopen error");
        return -1;
    }
    char* ptr = "change world\n";
    //size_t fwrite(const void *ptr, size_t size, size_t nmm, FILE *stream);
    fwrite(ptr,strlen(ptr),1,fp); 
    char buf[1024]={0};
    fseek(fp, 0, SEEK_SET);
    fread(buf,1023,1,fp);
    printf("read buf[%s]",buf);
    fclose(fp);
    return 0;
}

