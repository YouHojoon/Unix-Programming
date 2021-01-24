#include <stdio.h>
#include <sys/times.h>
#include <limits.h>
#include <unistd.h>
#include <stdlib.h>

#define CLK_TCK ((clock_t)sysconf(_SC_CLK_TCK))//시스템 정보에서 클록 틱수를 저장한다

int main()
{
        int i;
        time_t t;
        struct tms tms;
        clock_t start,end;
        
        if((start = times(&tms))==-1){//시작한 시간을 start에 저장하고 tms에 프로세스 실행 시간을 저장하는데 오류가 생긴다면
            perror("times");
            exit(1);
        }

        for(i = 0; i <= 1000000; i++)
                printf("%d\n", i);

        if((end = times(&tms))==-1){//반복문이 끝난 시간을 end에 저장하는데 tms에 프로세스 실행 시간을 저장하는데  오류가 생긴다면
            perror("times");
            exit(1);
        }

        printf("real : %.3f sec\n", (double)(end-start)/CLK_TCK);//끝난 시간과 시작한 시간을 빼서 전체 프로세스의 수행시간을 출력한다
        printf("user : %.3f sec\n", (double)(tms.tms_utime)/CLK_TCK);//유저 영역에서 수행한 시간을 출력한다
        printf("sys : %.3f sec\n", (double)(tms.tms_stime)/CLK_TCK);//커널 영역에서 수행한 시간을 출력한다

        return 0;
}
