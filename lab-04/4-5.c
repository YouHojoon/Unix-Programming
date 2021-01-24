#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

int main(void){
    if(mkdir("linux", 0755) == -1){//0755의 권한을 가지는 linux라는 디렉터리를 만드는 것에 실패하면
        perror("linux");
        exit(1);
    }
    
    if(mkdir("programming", 0644) == -1){//0644의 권한을 가지는 programming라는 디렉터리를 만드는 것에 실패하면
        perror("programming");
        exit(1);
    }

    return 0;
}
