#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int main(void){
    int pd[2],pid;
    char str[] = "Pipe Test";
    char buf[256];
    int len, status;

    if(pipe(pd) ==-1){//파이프를 만드는데 실패 시
        perror("pipe");
        exit(1);
    }
    
    switch(pid=fork()){
        case -1:
            perror("fork()");
            exit(1);
        case 0:
            close(pd[1]);//쓰기용 파이프는 필요 없으므로 닫는다
            len = read(pd[0], buf, 256);//읽기용 파이프로 데이터를 읽는다
            buf[len] = '\0';
            printf("Parent send %s from pipe.\n",buf);
            close(pd[0]);
            exit(2);
            break;
        default:
            close(pd[0]);//읽기용 파이프는 필요 없으므로 닫는다
            write(pd[1], str, strlen(str));//쓰기용 파이프로 str을 전송한다
            printf("Parent writes %s to pipe.\n",str);
            wait(NULL);//자식 프로세스가 끝나기를 기다린다
            close(pd[1]);
            break;
    }

    
    return 0;
}
