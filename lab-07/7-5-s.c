#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int main(void){
    int pd, n;
    char msg[] = "Hello, FIFO";

    printf("Server =====\n");

    if(mkfifo("./TESTFIFO", 0666) ==-1){//FIFO파일을 만드는데 실패할 경우
        perror("mkfifo");
        exit(1);
    }
    
    if((pd = open("./TESTFIFO", O_WRONLY))==-1){//파일을 쓰기 전용으로  여는데 실패한 경우
        perror("open");
        exit(1);
    }

    printf("To Client : %s\n", msg);
    n = write(pd, msg, strlen(msg)+1);//FIFO파일에 데이터를 쓴다
    if(n==-1){
        perror("write");
        exit(1);
    }

    close(pd);
}
