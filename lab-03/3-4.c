#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>

int main(void){
    int fd,n;
    off_t start, cur;
    char buf[256];
    
    fd = open("3-1.txt", O_RDONLY);//3-1.txt의 읽기 전용 파일 디스크립터를 얻는다

    if(fd==-1){//3-1.txt의 열기에 실패했다면
        perror("Open 3-1.txt");
        exit(1);
    }
    
    start = lseek(fd,0, SEEK_CUR);//fd의 현재 파일 오프셋을 받는다

    n = read(fd, buf, 255);//파일의 내용을 255 바이트만큼 읽어 buf에 저장한다
    buf[n]='\0';
    printf("Offset start=%d, Read Str=%s, n=%d\n", (int)start, buf, n);
    cur = lseek(fd, 0, SEEK_CUR);//fd의 현재 파일 오프셋을 받는다
    printf("Offset cur=%d\n", (int)cur);

    start = lseek(fd, 7, SEEK_SET);//fd의 오프셋을 7로 설정한다
    n = read(fd, buf, 255);//파일의 내용을 255 바이트만큼 읽어 buf에 저장한다
    buf[n]='\0';
    printf("Offset start=%d, Read Str=%s, n=%d\n", (int)start, buf, n);
   
    close(fd);

    return 0;
}
