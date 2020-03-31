#include <stdlib.h>
#include <stdio.h>
#include <time.h>
int main()
{
    for(clock_t start=clock(); clock()-start< 101*1000; );
    return 0;
}