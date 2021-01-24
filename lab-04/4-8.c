#include <dirent.h>
#include <stdlib.h>
#include <stdio.h>

int main(void){
    DIR *dp;
    struct dirent *dent;

    if((dp = opendir(".")) == NULL){//현재 디렉토리를 여는 것을 실패한다면
        perror("opendir: .");
        exit(1);
    }

    while((dent=readdir(dp))){//디렉토리의 내용을 끝까지 반복하며
        printf("Name : %s ", dent->d_name);
        printf("Inode : %d\n", (int)dent->d_ino);
    }

    closedir(dp);

    return 0;
}
