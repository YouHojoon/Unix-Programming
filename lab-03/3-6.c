#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void){
    char *fname;
    char fntmp[BUFSIZ];
    char template[32];

    fname = tmpnam(NULL);//fname에 임시파일명을 생성하여 저장
    printf("1. TMP File Name(tmpnam) : %s\n", fname);

    tmpnam(fntmp);//fntmp에 임시파일명 생성하여 저장
    printf("2. TMP File Name(tmpnam) : %s\n", fntmp);

    fname = tempnam("/tmp", "coredump");// /tmp 디렉토리 밑에 접두어 cored로(접두어는 5자리까지 지원) 임시파일명을 생성하여 저장
    printf("3. TMP File Name(tmpnam) : %s\n", fname);

    strcpy(template, "/tmp/coredumpXXXXXX");
    fname = mktemp(template);// /tmp 디렉터리에 coredump와 랜덤한 6자리로 임시파일명을 생성하여 저장한다
    printf("4. TMP File Name(mktemp) : %s\n", fname);
    
    return 0;
}
