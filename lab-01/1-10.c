#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char* argv[]){

    int i=0,result,current_optind;
    char n;
    int s[argc][3];
    extern char *optarg;
    extern int optind, opterr,optopt;
    if(*(argv+1)[0]!='-'){//옵션이 없을 시
        result=0;
        for(i=0; i<argc; i++)//인자의 개수만큼
            result+=atoi(argv[i]);//인자를 int형으로 변환하여 결과에 더한다
        printf("%d\n",result);
        exit(1);
    }
    opterr=0;//getopt에 지정된 오류 메시지가 나오지 않게 하기 위해 0으로 지정
    current_optind=optind;
    while((n=getopt(argc,argv,"am"))!=-1){
        switch(n){
            case 'a':
                result=0;
                for(i=1; *(argv+current_optind+i)!=NULL&&*(argv+current_optind+i)[0]!='-'; i++)//그 다음 옵션이 아닐 때 까지
                    result+=atoi(argv[current_optind+i]);//인자를 int형으로 변환하여 result에 더한다
                printf("%d\n",result);
                if(current_optind!=optind)//붙어있는 옵션을 다 처리했을 때
                    current_optind+=i;//i를 더해서 다음 옵션의 index로 만든다
                break;
            case 'm':
                result=1;
                for(i=1; *(argv+current_optind+i)!=NULL&&*(argv+current_optind+i)[0]!='-'; i++)//그 다음 옵션이 아닐 때 까지
                    result*=atoi(argv[current_optind+i]);//인자를 int형으로 변환하여 result에 더한다
                printf("%d\n",result);
                if(current_optind!=optind)//붙어있는 옵션을 다 처리했을 때
                    current_optind+=i;//i를 더해서 다음 옵션의 index로 만든다
                break;
            default:
                printf("1-9: 부적절한 옵션 -- '%c'\n",optopt);
                printf("Useage: 1-9 [-am] n1 n2\n");
                exit(1);
        }
    }
    return 0;
}
