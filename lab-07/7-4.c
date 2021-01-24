#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <string.h>

int main(void){
    int pd1[2],pd2[2],pid;
    char str[256];
    char buf[256];
    int len, status;

    if(pipe(pd1) ==-1){//파이프를 만드는데 실패 시
        perror("pipe");
        exit(1);
    }
    
    if(pipe(pd2) ==-1){//파이프를 만드는데 실패 시
        perror("pipe");
        exit(1);
    }
    
    switch(pid=fork()){
        case -1:
            perror("fork()");
            exit(1);
        case 0:
            close(pd1[1]);//pd1의 쓰기용 파이프는 필요 없으므로 닫는다
            close(pd2[0]);//pd2의 읽기용 파이프는 필요 없으므로 닫는다
            
            strcpy(str,"from child");
            write(pd2[1], str, strlen(str));//쓰기용 파이프로 str을 전송한다
            
            len=read(pd1[0], buf, sizeof(buf));
            buf[len]='\0';
            printf("Parent send %s to pipe.\n",buf);

            close(pd1[0]);
            close(pd2[1]);
            exit(2);
        default:
            close(pd1[0]);//pd1의 읽기용 파이프는 필요 없으므로 닫는다
            close(pd2[1]);//pd2의 쓰기용 파이프는 필요 없으므로 닫는다
            
            strcpy(str,"from parent");
            write(pd1[1], str, strlen(str));//쓰기용 파이프로 str을 전송한다
            
            len=read(pd2[0], buf, sizeof(buf));
            buf[len]='\0';
            printf("Child send %s to pipe.\n",buf);

            wait(NULL);//자식 프로세스가 끝나기를 기다린다
            close(pd2[0]);
            close(pd1[1]);
            break;
    }

    
    return 0;
}
