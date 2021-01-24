#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <unistd.h>

int main(void){
    struct stat buf;

    stat("password.txt", &buf);//password.txt의 정보를 buf에 저장한다 
    printf("Link Count before link() = %d\n", (int)buf.st_nlink);
    link("password.txt", "password.ln");//password.txt에 대한 하드링크를 password.ln으로 만든다

    stat("password.txt", &buf);
    printf("Link Count after link() = %d\n", (int)buf.st_nlink);
    
    stat("password.ln", &buf);//password.ln의 정보를 buf에 저장한다 
    printf("Link Count of password.ln = %d\n", (int)buf.st_nlink);

    symlink("password.txt", "password.sln");//password.txt에 대한 심볼릭 링크를 password.sln으로 만든다
    
    stat("password.txt", &buf);
    printf("Link Count of password.txt = %d\n", (int)buf.st_nlink);
    
    stat("password.sln", &buf);//password.sln 원본 파일의 정보를 buf에 저장한다 
    printf("Link Count of password.sln = %d\n", (int)buf.st_nlink);
    
    lstat("password.sln", &buf);//password.sln의 자기 자신의  정보를 buf에 저장한다 
    printf("Link Count of password.sln itself = %d\n", (int)buf.st_nlink);
    
    return 0;
}
