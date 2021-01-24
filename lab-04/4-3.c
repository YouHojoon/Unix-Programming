#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>

int main(void){
    struct stat buf;

    stat("password.txt", &buf);//password.txt의 정보를 buf에 저장한다 
    printf("mode before change = %o\n", (unsigned int)buf.st_mode);

    buf.st_mode |= S_IWGRP;//그룹의 쓰기 권한을 추가한다
    buf.st_mode &= ~(S_IROTH);//다른 인원의 읽기 권한을 제거한다
    chmod("password.txt", buf.st_mode);
    stat("password.txt", &buf);

    printf("mode after change = %o\n", (unsigned int)buf.st_mode);

    return 0;
}
