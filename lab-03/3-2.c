#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <string.h>

int main(void){
    int fd;
    int n;
    
    char str[]="UNIX SYSTEM PROGRAMMING";

    fd = open("3-1.txt", O_WRONLY);//3-1.txt의 쓰기 전용 파일 디스크립터를 얻는다

    if(fd==-1){//open함수가 오류를 발생시킨다면
        perror("Open");
        exit(1);
    }

    n=write(fd,str,strlen(str));//str을 fd의 파일에 str길이 만큼 쓴다

    if(n!=strlen(str)){//write함수가 입력한 길이가 str의 길이와 다르다면
        perror("Write");
        exit(2);
    }

    close(fd);

    return 0;
}
