#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

int main(void){
    int p[2];
    pid_t pid;

    if(pipe(p)==-1){//pipe를 만드는데 실패할 시
        perror("pipe");
        exit(1);
    }

    switch(pid=fork()){
        case -1://자식 프로세스를 만드는데 실패할 경우
            perror("fork");
            exit(1);
        case 0:
            close(p[0]);//읽기 파이프 닫기
            if(p[1]!=1){//p[1]이 표준 출력이 아닐 시
                dup2(p[1],1);//p[1]을 표준 출력으로 복사
                close(p[1]);
            }
            execlp("ls","ls","-l",(char *)NULL);//ls -l명령어 실행
            exit(1);
            break;
        default:
            close(p[1]);//쓰기 파이프 닫기
            if(p[0]!=0){//p[0]이 표준 입력이 아니라면
                dup2(p[0],0);//p[0]을 표준 입력으로 복사
                close(p[0]);
            }
            wait(NULL);//자식 프로세스가 끝날 때 까지 대기
            execlp("grep","grep","c",(char *)NULL);//grep -c명령어 실행
            break;
    }
    
    return 0;
}
