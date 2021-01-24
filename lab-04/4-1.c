#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>

int main(void){
    struct stat buf;
    
    stat("password.txt", &buf);//passowrd.txt의 대한 정보를 buf에 저장한다

    printf("Inode = %d\n", (int)buf.st_ino);
    printf("Mode = %o\n", (unsigned int)buf.st_mode);
    printf("Nlink = %o\n", (unsigned int)buf.st_nlink);//하드 링크의 개수
    printf("UID = %d\n", (int)buf.st_uid);
    printf("GID = %d\n", (int)buf.st_gid);
    printf("SIZE = %d\n", (int)buf.st_size);
    printf("Atime = %d\n", (int)buf.st_atime);//마지막으로 접근한 시간
    printf("Mtime = %d\n", (int)buf.st_mtime);//마지막으로 수정한 시간
    printf("Ctime = %d\n", (int)buf.st_ctime);//마지막으로 inode의 내용을 변경한 시간
    printf("Blksize = %d\n", (int)buf.st_blksize);//파일의 내용을 입출력할 때 사용하는 버퍼의 크기
    printf("Blocks = %d\n", (int)buf.st_blocks);//512바이트씩 블록으로 나눈 개수



    return 0;
}
