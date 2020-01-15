#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
int main()
{
    
    printf("%d\n",stdin->_fileno);
    printf("%d\n",stdout->_fileno);
    printf("%d\n",stderr->_fileno);
    return 0;
}

