#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>

extern int errno;

int main(void){

    char *str;

    if(access("linux.txt", F_OK)==-1){//만약 linux.txt라는 파일이 존재하지 않는다면
        str=strerror(errno);//errno에 해당하는 오류 메시지를 str에 저장한다.
        printf("linux.txt: %s\n", str);
        exit(1);
    }
    return 0;
}
