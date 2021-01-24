#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <stdio.h>

int main(void){
    printf("Before SIGCONT Signal to parent.\n");

   kill(getppid(), SIGCONT);//부모 프로세스에 SIGCONT 시그널을 보낸다

   printf("Before SIGQUIT Signal to me.\n");
    
   kill(getpid(), SIGQUIT);//SIGQUIT 시그널을 자신에게 보낸다

   printf("After SIGQUIT Signal.\n");

   return 0;
}
