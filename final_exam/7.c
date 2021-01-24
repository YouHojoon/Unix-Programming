#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <string.h>

int main(void){
    pid_t pid;
    int shmid;
    char *addr, *addr2;

            shmid = shmget(IPC_PRIVATE, 1024, IPC_CREAT|0644);//키는 IPC_PRIVATE로 공유메모리를 생성한다
            
            if(shmid==-1){//공유 메모리 생성에 실패할 시
                perror("shmget");
                exit(1);
            }

    switch(pid=fork()){
        case -1://자식 프로세스를 만드는데 실패할 시
            perror("fork");
            exit(1);
        case 0:
            printf("=Child begins\n");
            char *msg = "Message from child";            
            addr2 = (char *)shmat(shmid,(char*)NULL,0);//공유 메모리에 연결한다
            strcpy(addr2,msg);
            shmdt(addr2);//공유 메모리에 연결을 해제한다
            
            printf("=Child ends\n");
            exit(1);
            break;
        default:
            wait(NULL);//자식 프로세스가 끝나기를 기다린다
            printf("=Parent begins\n");
            addr=(char *)shmat(shmid,(char *)NULL,0);//공유 메모리에 연결한다
            printf("%s\n",addr);
            shmdt(addr);//공유 메모리에 연결을 해제한다

            printf("=Parent ends\n");
            break;
    }

    return 0;
}
