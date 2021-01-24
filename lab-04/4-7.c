#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

int main(void){

    if(rmdir("programming")==-1){//programming을 삭제하는 데 실패한다면
        perror("programming");
        exit(1);
    }
    
    return 0;
}
