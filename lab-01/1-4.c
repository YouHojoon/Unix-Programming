#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>

extern int errno;

int main(void){
    if(access("linux.txt", F_OK)==-1){//만약 linux.txt라는 파일이 존재하지 않는다면
        perror("linux.txt");//linux.txt를 출력하고 errno의 값에 해당되는 오류의 메시지를 출력한다
        exit(1);
    }
    return 0;
}
