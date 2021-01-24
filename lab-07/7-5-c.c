#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main(void){
    int pd, n;
    char inmsg[80];

    if((pd=open("./TESTFIFO", O_RDONLY))==-1){//파일을 읽기전용으로 여는데 실패할 경우
        perror("open");
        exit(1);
    }

    printf("Client ====\n");
    write(1, "From Server :", 13);

    while((n=read(pd,inmsg,80))>0)//FIFO파일로부터 데이터를 읽는다
        write(1,inmsg,n);//stdout에 inmsg의 내용을 입력

    if(n==-1){
        perror("read");
        exit(1);
    }

    write(1, "\n",1);
    close(pd);

    return 0;
}
