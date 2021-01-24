#include <sys/types.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

void sock_read(int socket, char *buf);
void sock_write(int socket, char *buf);

int main(int argc, char *argv[])
{
    int sock;
    char buf[BUFSIZ];
    struct sockaddr_in srvAddr;
    pid_t pid;

    if (argc != 3)
    { //IP와 포트가 인자로 들어오지 않았을 시
        printf("Usage %s <IP> <PORT>\n", argv[0]);
        exit(1);
    }

    if ((sock = socket(PF_INET, SOCK_STREAM, 0)) == -1)
    { //소켓을 IPv4 인터넷 프로토콜, TCP소켓으로 생성하는데 오류가 생겼을 시
        perror("socket()");
        exit(2);
    }
    memset(&srvAddr, 0, sizeof(srvAddr));
    srvAddr.sin_family = AF_INET;                 //IPv4주소 체계로 지정
    srvAddr.sin_addr.s_addr = inet_addr(argv[1]); //인자로 들어온 IP로 지정
    srvAddr.sin_port = htons(atoi(argv[2]));      //인자로 들어온 포트로 지정

    if (connect(sock, (struct sockaddr *)&srvAddr, sizeof(srvAddr)) == -1)
    { //서버에 연결하는데 오류가 생겼을 시
        perror("connet()");
        exit(2);
    }

    printf("관리자 비밀번호를 입력해주세요 : ");
    fgets(buf, BUFSIZ, stdin);
    write(sock, buf, strlen(buf));

    memset(buf, 0, sizeof(buf));
    read(sock, buf, BUFSIZ);

    if (strcmp(buf, "fail") == 0)
    { //서버에 지정된 비밀번호와 맞지 않을 시
        printf("관리자 비밀번호가 맞지 않습니다.\n");
        close(sock);
        exit(3);
    }

    /*
        관리자로 로그인 성공 시
    */

    printf("%s", buf);
    memset(buf, 0, sizeof(buf)); //명령을 입력하기 전 버퍼 초기화

    pid = fork();

    if (pid == 0)//명령 입력 프로세스
    { 
        sock_write(sock, buf);
        close(sock);
        exit(1);
    }
    else //명령 수행 결과 읽기 프로세스
        sock_read(sock, buf);

    close(sock);
    return 0;
}

void sock_read(int socket, char *buf)
{
    int readCnt;

    while (1)
    {
        readCnt = read(socket, buf, BUFSIZ);

        if (readCnt == 0)
            return;
        buf[readCnt] = '\0';
        printf("%s", buf);
    }
}

void sock_write(int socket, char *buf)
{
    while (1)
    {
        fgets(buf, BUFSIZ, stdin);
        write(socket, buf, strlen(buf));

        if (strcmp(buf, "logout\n") == 0)
        {
            shutdown(socket, SHUT_WR);
            return;
        }
    }
}