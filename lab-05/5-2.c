#include <pwd.h>
#include <stdio.h>

int main(void){
    struct passwd *pw;

    pw = getpwnam("dbghwns11");//dbghwns11의 /etc/passwd 엔트리를 읽어들임
    printf("UID : %d\n", (int)pw->pw_uid);//uid출력
    printf("Name : %s\n", pw->pw_name);//로그인 네임 출력
    printf("Home Dir : %s\n", pw->pw_dir);//홈 디렉터리 출력
    printf("Shell : %s\n", pw->pw_shell);//로그인 할 때 사용하는 셸 출력

    return 0;
}
