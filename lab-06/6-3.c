#include <unistd.h>
#include <stdio.h>
#include <signal.h>

int main(void){
    int i;

    sighold(SIGINT);//SIGINT 시그널을 블록한다

    for(i=0; i<10; i++){
        printf("sleep 1 second...\n");
        sleep(1);//1초동안 스레드를 멈추게 한다
    }
    
    return 0;
}
