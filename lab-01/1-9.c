#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[]){
    int i,result,j=1,k=0;
    char n;
  
   if(*(argv+1)[0]!='-'){//옵션이 없을 시
       result=0;
        for(i=0; i<argc; i++)//인자의 개수만큼
            result+=atoi(argv[i]);//인자를 int형으로 변환하여 결과에 더한다
        printf("%d\n",result);
        exit(1);
   }
  
   while((*++argv)){//옵션이 존재할 때 까지
    while(n=*(argv[0]+j)){//옵션의 j번째 글자가 존재할 때 까지
        k=0;
        switch(n){
            case 'a':
                result=0;
                for(i=1; *(argv+i)!=NULL&&*(argv+i)[0]!='-'; i++){//인자가 존재하고 옵션이 아닐 때 까지
                    result+=atoi(argv[i]);//결과에 인자를 int형으로 변환하여 더한다
                    k++;
                }
                printf("%d\n",result);
                j++;
                break;
            case 'm':
                result=1;
                for(i=1; *(argv+i)!=NULL&&(*argv+i)[0]!='-'; i++){//인자가 존재하고 옵션이 아닐 때 까지
                    result*=atoi(argv[i]);//결과에 인자를 int형으로 변환하여 곱한다
                    k++;
                }
                printf("%d\n",result);
                j++;
                break;
          default:
                printf("1-9: 부적절한 옵션 -- '%c'\n",n);
                printf("Useage: 1-9 [-am] n1 n2\n");
                exit(1);
        }
    }
    argv+=k;//다음 옵션으로 넘어가기 위해서 인자만큼 주소에 더 해준다.
    j=1;
   }

    return 0;
}
