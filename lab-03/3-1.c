#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>

int main(void){
    int fd;
    mode_t mode;

    mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;//소유자는 읽기 ,쓰기 권한 그룹과 다른 사람에게는 읽는 권한을 부여한다
    fd = open("3-1.txt", O_CREAT, mode);//만약 3-1.txt가 없다면 생성한다

    if(fd==-1){//open함수가 오류를 발생시킨다면
        perror("Creat");
        exit(1);
    }

    close(fd);

    return 0;
}
