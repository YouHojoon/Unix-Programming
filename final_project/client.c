#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <pthread.h>

#define NAME_LENGTH 11//이름 길이 제한

void *send_msg(void *arg);
void *recv_msg(void *arg);

char name[NAME_LENGTH];

int main(int argc, char *argv[])
{
    int sock;
    struct sockaddr_in srvAddr;
    pthread_t send_thread, rcv_thread;
    void *thread_return;

    if (argc != 3)
    {
        printf("Usage %s <IP> <PORT>\n", argv[0]);
        exit(1);
    }
   
    if((sock = socket(PF_INET, SOCK_STREAM, 0))==-1){//sock을 IPv4 TCP소켓으로 만드는데 실패할 경우
        perror("socket");
        exit(1);
    }

    memset(&srvAddr, 0, sizeof(srvAddr));
    srvAddr.sin_family = AF_INET;//IPv4 설정
    srvAddr.sin_addr.s_addr = inet_addr(argv[1]);//IP설정
    srvAddr.sin_port = htons(atoi(argv[2]));//포트 설정

    if (connect(sock, (struct sockaddr *)&srvAddr, sizeof(srvAddr)) == -1){//서버와 연결하는데 실패할 경우
        perror("connect");
        exit(1);
    }
        
    char *buff=(char*)malloc(255*sizeof(char));
    int strLen;

    while((strLen = read(sock, buff, sizeof(buff)))!=0){//방 목록 불러오기
        buff[strLen] = '\0';
        printf("%s",buff);
        if(strLen < 8)
            break;
    }

    printf("접속하실 방 번호를 입력하여 주세요.\n");
    fgets(buff,sizeof(buff),stdin);
    write(sock,buff,strlen(buff));
    
    strLen  = read(sock,buff,sizeof(buff));
    buff[strLen] = '\0';

    if(strcmp(buff,"0\n")==0){
        /*
            올바르지 않은 방 번호를 입력했을 시
         */
        printf("방 번호를 올바르게 입력하여 주세요.\n");
        free(buff);
        close(sock);
        exit(1);
    }

    while(1){
        /*
            닉네임 입력
         */
        printf("닉네임(영어 8자 이내) : ");
        fgets(name,sizeof(name),stdin);
        name[strlen(name)-1]='\0';
        write(sock, name, strlen(name));
         
        if(strcmp(name,"!exit")==0){// 도중에 나갈 시
            close(sock);
            exit(1);
        }

        
        strLen=read(sock,buff,sizeof(buff));
        buff[strLen]='\0';
        if(strcmp(buff,"0\n")==0){//중복되는 닉네임이라고 서버에서 반환 시
            printf("이미 존재하는 닉네임입니다.\n");
            continue;
        }
        else
            break;
    }

    free(buff);

    pthread_create(&send_thread, NULL, send_msg, (void *)&sock);//입력 스레드
    pthread_create(&rcv_thread, NULL, recv_msg, (void *)&sock);//출력 스레드
    
    pthread_join(send_thread,NULL);//입력 스레드에서 종료를 다루므로 JOIN
    pthread_detach(rcv_thread);
    
    close(sock);
    return 0;
}
void *send_msg(void *arg)
{
    int sock = *((int *)arg);
    char buff[BUFSIZ];
    
    while (1)
    {
        fgets(buff, sizeof(buff), stdin);
        write(sock, buff, strlen(buff));

        if(strcmp(buff,"!exit\n")==0){
            shutdown(sock, SHUT_WR);
            return NULL;
        }
    }
}

void *recv_msg(void *arg)
{
    int sock = *((int *)arg);
    char buff[BUFSIZ];
    int strLen;
    
    while (1)
    {
        strLen = read(sock, buff, sizeof(buff));
        buff[strLen] = '\0';
        printf("%s",buff);
    }
}

