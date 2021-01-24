#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>

void handler(int signo){
    psignal(signo, "Received Signal:");
    sleep(5);
    printf("In Signal Handler, After Sleep\n");
}

int main(void){
    struct sigaction act;

    sigemptyset(&act.sa_mask);//시그널 집합을 비운다
    act.sa_flags = 0;
    act.sa_handler = handler;//시그널 집합의 핸들러를 handler() 함수로 지정한다
    if(sigaction(SIGINT, &act, (struct sigaction *)NULL) <0){//SIGINT 시그널의 처리를 act로 지정하는데 실패할 경우
            perror("sigaction");
            exit(1);
    }

    while(1){
        fprintf(stderr, "Input SIGINT: ");
        pause();
        fprintf(stderr, "After Signal Handler\n");
    }

    return 0;

}
