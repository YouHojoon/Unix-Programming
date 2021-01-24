#include <stdio.h>
#include <signal.h>

int main(void){
    short i=0;
    
    sighold(SIGINT);//SIGINT시그널을 블럭

    while(1){
        if(i<0)
            i=9;
        printf("%d\r", i++);
    }

    return 0;
}
