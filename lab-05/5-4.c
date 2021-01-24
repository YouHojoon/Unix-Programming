#include <stdio.h>
#include <unistd.h>

int main(void){
    printf("PID : %d\n", (int)getpid());//자신의 pid를 출력한다
    printf("PPID : %d\n", (int)getppid());//자신의 부모 프로세스의 pid를 출력한다

    return 0;
}
