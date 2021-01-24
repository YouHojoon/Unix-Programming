#include <stdio.h>
#include <stdlib.h>

int main(void){

    if(rename("linux","LINUX")==-1){//linux의 이름을  LINUX로 바꾸는 것에 실패한다면
        perror("linux");
        exit(1);
    }
    
    return 0;
}
