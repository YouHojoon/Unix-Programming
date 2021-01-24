#include <stdlib.h>
#include <stdio.h>

int main(void){
    FILE *fp;
    char buf[256];
    
    fp = popen("ps","r");//파이프를 읽기 전용으로 생성하고 ps실행
    if(fp == NULL){//파이프를 생성하는데 실패 시
        fprintf(stderr, "popen failed\n");
        exit(1);
    }

    
    while(fgets(buf, sizeof(buf), fp)!=NULL)//fp에서 받아온 데이터가 없을 때 까지
        printf("%s", buf);

    pclose(fp);

    return 0;
}
