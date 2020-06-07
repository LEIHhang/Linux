/*************************************************************************
*File Name: Look_for_the_KTH_decimal.cpp
*Author: Leihang
*mail: 973331055@qq.com
*Created Time: 2020年06月07日 星期日 12时13分05秒
*describe: 
************************************************************************/
#include <iostream>
using namespace std;

int Partition(int*& a,int l,int r)
{
    int i = l,j = r+1;
    int x = a[l];
    while(true)
    {
        //找到第一个比他小的
        while(a[++i] < x && i <= r);
        //找到第一个比他大的
        while(a[--j] > x);
        if(i >= j)
            break;
        //交换大的和小的
        int m;
        m = a[i];
        a[i] = a[j];
        a[j] = m;
    }
    a[l] = a[j];
    a[j] = x;
    //返回之前元素第一个数的下标
    return j;
}

int select(int *a,int left, int right, int k)
{
    if(left == right)
        return a[left];
    int i = Partition(a, left, right);
    int j = i - left + 1;
    if(k <= j)
        return select(a, left, i, k);
    else
        return select(a, i+1, right, k-j);
}

int main(int argc,char* argv[])
{
    int m,k;
    cin>> m ;
    int* p = new int[m];
    for(int i= 0; i< m;i++)
    {
        cin>>p[i];
    }
    cin>>k;
    int h = select(p,0,m-1,k);
    cout << h;
    return 0;
}

