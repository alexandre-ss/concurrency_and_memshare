#include <stdio.h>
#include <time.h>
#include <stdlib.h>
int main(){
    time_t seconds;
    
    srand(time(NULL));
    int r = rand();
    printf("%d", r%4);
    return 0;
}