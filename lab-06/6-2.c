#include <unistd.h>
#include <stdio.h>

int main(void){
    int i;

    for(i=0; i<10; i++){
        printf("sleep 1 second...\n");
        sleep(1);//1초동안 스레드를 멈추게 한다
    }
    
    return 0;
}
