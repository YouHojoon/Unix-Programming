#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(void){
    char *str;

   // str=malloc(sizeof(char)*20);//char크기(1 바이트)*20만큼 메모리 할당

    strcpy(str,"Hello");//Hello라는 문자열을 str로 복사
    printf("%s\n", str);

    strcpy(str, "Good morning");//Good morning이라는 문자열을 str로 복사
    printf("%s\n",str);

    free(str);//str에 할당한 메모리 해제
    return 0;
}
