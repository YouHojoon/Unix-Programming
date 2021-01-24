#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[]){
    int i,sum=0;

    for(i=1; i<argc; i++)//명령행  인자의 개수만큼 반복하여
        sum+=atoi(argv[i]);//문자열인 인자를 int형으로 변환하여 더한다
    printf("%d\n",sum);
    return 0;
}
