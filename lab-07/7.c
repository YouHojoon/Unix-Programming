#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct student{
    char name[255];
    int s1,s2,s3,sum;
    double avg;
}STUDENT;

int compare(const void *a, const void *b){
    STUDENT s1 = *(STUDENT *)a;
    STUDENT s2 = *(STUDENT *)b;
/*
    내림차순이므로 합이 클 때 -1을 반환하고 합이 작을 때 1을 반환해야 한다
 */
    if(s1.sum > s2.sum)
        return -1;
    
    if(s1.sum < s2.sum)
        return 1;
    
    return 0;
}
int main(void){
    int i,s1,s2,s3,cnt=0,s1_sum,s2_sum,s3_sum;
    char name[255];
    STUDENT students[255];
    
    FILE *rfp = fopen("7.dat","r");
    FILE *wfp = fopen("7.out","w+");//w+는 쓰기 전용으로 열 되 파일이 없을 경우 생성한다

    if(rfp == NULL){//7.dat을 읽기 전용 파일 포인터로 받는 데 실패한다면
        perror("7.dat fopen");
        exit(1);
    }

    if(wfp == NULL){//7.out을 쓰기 전용 파일 포인터로 받는데 실패한다면
        perror("7.out fopen");
        exit(1);
    }
    
    while(1){
        if(fscanf(rfp,"%s %d %d %d", name, &s1, &s2, &s3)!=EOF){//파일의 끝까지 형식대로 읽는다
            STUDENT student;
        
            strcpy(student.name, name);
            student.s1 = s1;
            student.s2 = s2;
            student.s3 = s3;
            student.sum = s1+s2+s3;
            student.avg = (double)student.sum / 3.0;
            students[cnt++] = student;
        }
        else
            break;
    }
    
    qsort(students,cnt,sizeof(STUDENT),compare);//compare함수를 이용하여 비교하며  내림차순으로 퀵소트한다
    fprintf(wfp, "%6s%12s%12s%12s%12s%12s\n", "Name","Score1","Score2","Score3","SUM","AVG");
    
    for(i = 0; i<cnt; i++){
        s1_sum += students[i].s1;
        s2_sum += students[i].s2;
        s3_sum += students[i].s3;

        fprintf(wfp,"%6s%12d%12d%12d%12d%12f\n",students[i].name, students[i].s1, students[i].s2, 
                students[i].s3, students[i].sum,students[i].avg);
    }
    fprintf(wfp, "------------------------------------------------------------------\n");
    fprintf(wfp, "%6s%12f%12f%12f\n", "AVG",(double)s1_sum / i, (double)s2_sum / i, (double)s3_sum / i);

    return 0;
}
