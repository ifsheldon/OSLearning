#include <unistd.h>
#include <stdio.h>
int main()
{
    while(1)
    {
        usleep(500*1000); //sleep 500ms
        printf("output\n");
    }
    return 0;
}