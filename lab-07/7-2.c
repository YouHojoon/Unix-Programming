#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int main(void){
    int pd[2];
    char str[] = "Pipe Test";
    char buf[256];
    int len, status;

    if(pipe(pd) ==-1){//파이프를 만드는데 실패 시
        perror("pipe");
        exit(1);
    }

    write(pd[1], str, strlen(str));//쓰기용 파이프로 str을 전송한다
    printf("Process %d writes %s to pipe.\n",(int)getpid(),str);
    len = read(pd[0], buf, 256);//읽기용 파이프로 데이터를 읽는다
    buf[len] = '\0';
    printf("Process %d reads %s from pipe.\n",(int)getpid(),buf);

    close(pd[0]);
    close(pd[1]);
    
    return 0;
}
