#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

void handler(){
    printf("catch SIGINT ...\n");
}

int main(void){
    int i;
    
    if(sigset(SIGINT, handler) == SIG_ERR){//SIGINT 시그널 핸들러를 지정하는데 실패할 시
        perror("sigset");
        exit(1);
    }

    for(i=0; i<10; i++){
        printf("sleep 1 second...\n");
        sleep(1);//1초동안 스레드를 멈추게 한다
    }
    
    return 0;
}
