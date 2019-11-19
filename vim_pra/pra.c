#include <stdio.h>
int num=0;
int test()
{
    if(num==1)
    {
        printf("今天天气真好\n");
        return 1;
    }
    else
    {
        printf("今天天气很不好\n");
        return 0;
    }
}
void test2(int a)
{
    if(a==1)
    {
        printf("那我们出去玩吧\n");
    }
    else
    {
        printf("那还是在家呆着吧\n");
    }
}
int main()
{
    test2(test());
    printf("Hello world\n");
    return 0;
}

