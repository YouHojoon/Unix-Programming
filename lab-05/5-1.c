#include <unistd.h>
#include <stdio.h>

int main(void){
    printf("Max argv and envp : %ld\n", sysconf(_SC_ARG_MAX));//arg[]와 env[]를 합한 최대 크기를 바이트 단위로 출력한다
    printf("Max child : %ld\n", sysconf(_SC_CHILD_MAX));//한 uid에 허용되는 최대 프로세스 개수를 출력한다
    printf("Clock Tick : %ld\n", sysconf(_SC_CLK_TCK));//초당 클록 틱 수를 출력한다
    printf("Max Open File : %ld\n", sysconf(_SC_OPEN_MAX));//프로세스당 열 수 있는 최대 파일 개수를 출력한다
    printf("Version : %ld\n", sysconf(_SC_VERSION));//posix의 버전을 출력한다

    return 0;
}
   
