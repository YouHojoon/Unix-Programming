#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>

int main(void){
    struct stat buf;

    stat("password.txt" , &buf);//password.txt의 정보를 buf에 저장
    
    printf("MOde = %o (16진수 : %x)\n", (unsigned int)buf.st_mode, (unsigned int)buf.st_mode);

    if(S_ISFIFO(buf.st_mode)) printf("password.txt: FIFO\n");//buf.st_mod & 0XF000이 0X1000일 때
    if(S_ISDIR(buf.st_mode)) printf("password.txt: Directory\n");//buf.st_mod & 0XF000이 0X4000일 때
    if(S_ISREG(buf.st_mode)) printf("password.txt: Regualr File\n");//buf.st_mod & 0XF000이 0X8000일 때

    return 0;
}
