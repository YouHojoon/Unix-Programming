#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

int main(void){
    int srvSock, clntSock,strLen;
    struct sockaddr_in srvAddr, clntAddr;
    socklen_t szAddr;
    char buf[BUFSIZ];
    
    if((srvSock=socket(PF_INET,SOCK_STREAM,0))==-1){//소켓을 IPv4 TCP소켓으로 만드는데 실패할 경우
        perror("socket");
        exit(1);
    }    

    memset(&srvAddr,0,sizeof(srvAddr));
    srvAddr.sin_family = AF_INET;//IPv4설정
    srvAddr.sin_addr.s_addr = inet_addr("127.0.0.1");//소켓의 IP를 localhost로 설정
    srvAddr.sin_port = htons(9000);//소켓의 포트를 9000으로 설정
    
    if(bind(srvSock, (struct sockaddr *)&srvAddr, sizeof(srvAddr))==-1){//소켓 지정에 실패할 경우
        perror("bind");
        exit(1);
    }

    if(listen(srvSock,5)==-1){//클라이언트 대기에 실패한 경우
        perror("listen");
        exit(1);
    }

    
    while(1){
        szAddr=sizeof(clntAddr);
        clntSock = accept(srvSock, (struct sockaddr *)&clntAddr, &szAddr);//클라이언트 연결 허용
        if(clntSock == -1)//잘못된 소켓일 경우
            continue;

        if((strLen=recv(clntSock,buf,sizeof(buf),0))==-1){//수신에 실패할 경우
            perror("recv");
            exit(1);
        }
        buf[strLen]='\0';

        for(int i=0; i<strlen(buf); i++){//대문자를 소문자로 소문자를 대문자로 바꾼다
            char *c = &buf[i];
            int diff = 'a' - 'A';

            if(*c>='A' && *c<= 'Z')
                *c+=diff;

            else if(*c>='a' && *c<='z')
                *c-=diff;
        }
        
        if(send(clntSock, buf,sizeof(buf),0)==-1){//전송에 실패할 경우
            perror("send");
            exit(1);
        }
        
        close(clntSock);
    }

    close(srvSock);
    return 0;
}
