#include <unistd.h>
#include <stdio.h>
#include <errno.h>

extern int errno;

int main(void){
    if(access("unix.txt", F_OK)==-1)//만약 unix.txt라는 파일이 존재하지 않는다면
        printf("errno=%d\n",errno);//에러 번호를 출력한다
    return 0;
}
