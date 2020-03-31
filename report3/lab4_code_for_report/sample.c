#include <stdlib.h>
#include <stdio.h>
#include <time.h>
int main()
{
    for(clock_t start=clock(); (double)((clock()-start)/CLOCKS_PER_SEC)< 2.1; );
    return 0;
}