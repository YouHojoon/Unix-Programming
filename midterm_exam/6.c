#include <time.h>
#include <stdio.h>

int main(void){
    struct tm *tm;
    time_t t;
    char buff[BUFSIZ];
    char *format = "%G. %m. %d. (%a) %T";
    
    time(&t);//초 단위로 현재 시간을 얻는다
    tm = localtime(&t);//초 단위로  얻은 시간을 지역 시간대를 기준으로 분해한다
    tzset();//현재 지역의 시간대로 시간대를  설정한다
    
    strftime(buff, sizeof(buff), format, tm);//tm에 저장된 시간 정보를 format에 지정한 형식대로 문자열로 만들어 buff에 저장한다
    printf("%s %s\n",buff, tzname[0]);

    return 0;

}
