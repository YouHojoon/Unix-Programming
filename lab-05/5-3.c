#include <time.h>
#include <stdio.h>

int main(void){
    char *output = "%G. %m. %d. (%a) %T"; 
    struct tm *tm;
    time_t t;
    char buf[BUFSIZ];
    
    time(&t);//초 단위로 현재 시간의 정보를 저장한다
    tm=localtime(&t);//지역 시간대를 기준으로 시간정보를 분해해 저장한다
    tzset();//시간대를 설정한다
   
    strftime(buf,sizeof(buf),output,tm);//output에 지정된 형식에 따라 buf에 문자열로 저장한다
    printf("%s %s\n",buf,tzname[0]);
   
    return 0;
}
