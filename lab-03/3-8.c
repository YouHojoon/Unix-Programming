#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct Student{
    char name[100];
    double arv;
    int sum;
}Student;//학생들의 합과 평균을 저장하기 위한 구조체

int compare(const void *a, const void *b){
    Student s1 = *(Student *)a;
    Student s2 = *(Student *)b;
/*
   내림차순이므로 s1의 합이 클 때라면 -1을 반환하고 합이 작다면 1을 반환한다
   */
    if(s1.sum > s2.sum)
        return -1;
    if(s1.sum < s2.sum)
        return 1;
    return 0;
}
int main(void){
    FILE * f;
    Student list[BUFSIZ];
    int cnt=0;
    int m,k,e,ms=0,ks=0,es=0;//국어, 수학, 영어의 점수와 그 합 들을 저장하기 위한 변수들
    char name[100];
    
    f=fopen("3-8.dat","r");
    if(f==NULL){//3-8.dat을 open하는 것에 실패했을 시
        perror("Open");
        exit(1);
    }

    fgets(name,sizeof(name),f);//맨 윗줄을 버리기 위하여

    while(1){
        Student s;

        if(fscanf(f,"%s %d %d %d",name,&k,&e,&m)!=EOF){//형식에 맞춰서 각 변수들을 초기화
            strcpy(s.name,name);//s.name에 name의 내용을 복사
            s.sum=m+k+e;
            ms+=m;
            es+=e;
            ks+=k;
            s.arv=(double)s.sum/3;
            list[cnt]=s;
            cnt++;
        }
        else{
            break;
        }
    }
    qsort(list, cnt,sizeof(Student),compare);//compare함수를 이용하여서 list를 내림차순으로 퀵소트
    for(int i=0; i<cnt; i++)
        printf("%s 합 : %d 평균: %lf\n",list[i].name,list[i].sum,list[i].arv);
    printf("국어 평균 : %lf\n",(double)ks/cnt);
    printf("영어 평균 : %lf\n",(double)es/cnt);
    printf("수학 평균 : %lf\n",(double)ms/cnt);
    
    fclose(f);
    return 0;

}
