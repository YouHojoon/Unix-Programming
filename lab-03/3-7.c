#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>

int main(){
    char template[] = "/tmp/tmpXXXXXX";
    int fd;

    fd=mkstemp(template);//template에 지정된 형식으로 임시 파일을 생성한다
    printf("TMP File Name : %s\n", template);
    close(fd);
    return 0;

}
