#include <stdio.h>
#include <time.h>

int main(void){

    time_t r;
    struct tm *t;

    r = time(NULL);
    t = localtime(&r);

    int h = t->tm_hour;

    if(h<12)
        printf("G M.\n");
    else if(h < 18)
        print("G A.\n");
    else
}
