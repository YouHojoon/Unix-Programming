#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <string.h>

#define BUF_SIZE 100

int main(void){
    int wfd,rfd;
    int w_cnt, r_cnt;
    
    char str[BUF_SIZE];

    rfd = open("3-1.txt", O_RDONLY);//3-1.txt의 읽기 전용 파일 디스크립터를 얻는다
    wfd = open("3-3.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);//3-3.txt의 쓰기 전용 파일 디스크립터를 얻는다. 만약 파일이 없다면 생성한다. 있다면 쓰기 전용 파일 디스크립터를 얻는다.

    if(rfd==-1){//3-1.txt의 열기에 실패했다면
        perror("3-1.txt Open");
        exit(1);
    }

    if(wfd==-1){//3-3.txt의 열기에 실패했다면
        perror("3-3.txt Open");
        exit(2);
    }
    
    while((r_cnt=read(rfd,str,BUF_SIZE))!=0)//파일의 끝이 나올 때 까지  BUF_SIZE씩 읽어서 str에 저장한다
       w_cnt = write(wfd,str,r_cnt);//r_cnt만큼 wfd의 파일에 str의 내용을 입력한다
    
    if(r_cnt==-1){//read함수에 오류가 발생하면
        perror("Read");
        exit(3);
    }

    if(w_cnt==-1){//write함수에 오류가 발생하면
        perror("Write");
        exit(4);
    }

    close(rfd);
    close(wfd);

    return 0;
}
