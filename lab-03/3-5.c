#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <string.h>

#define BUF_SIZE 100

int main(void){
    FILE *wfd,*rfd;
    int w_cnt;
    
    char str[BUF_SIZE];

    rfd = fopen("3-1.txt", "r");//3-1.txt의 읽기 전용 파일 디스크립터를 얻는다
    wfd = fopen("3-5.txt", "w+");//3-3.txt의 쓰기 전용 파일 디스크립터를 얻는다. 만약 파일이 없다면 생성한다.

    if(rfd==NULL){//3-1.txt의 열기에 실패했다면
        perror("3-1.txt Open");
        exit(1);
    }

    if(wfd==NULL){//3-5.txt의 열기에 실패했다면
        perror("3-3.txt Open");
        exit(2);
    }
    
    while(fgets(str,BUF_SIZE,rfd)!=NULL)//파일의 끝 혹은 오류가 나올 때 까지 BUF_SIZE씩 읽어서 str에 저장한다
       w_cnt = fputs(str,wfd);
    
    if(w_cnt==EOF){//write함수에 오류가 발생하면
        perror("Write");
        exit(3);
    }

    fclose(rfd);
    fclose(wfd);

    return 0;
}
