#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

int main(int argc, char* argv[]){
    int sock,strLen;
    struct sockaddr_in addr;
    char buf[BUFSIZ];
    
    if(argc < 2){//문자열이 인자로 들어오지 않았을 시
        printf("Usage %s <STRING> ...\n", argv[0]);
        exit(1);
    }

    if((sock=socket(PF_INET,SOCK_STREAM,0))==-1){//소켓을 IPv4 TCP소켓으로 만드는데 실패할 경우
        perror("socket");
        exit(1);
    }    

    memset(&addr,0,sizeof(addr));
    addr.sin_family = AF_INET;//IPv4설정
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");//소켓의 IP를 localhost로 설정
    addr.sin_port = htons(9000);//소켓의 포트를 9000번으로 설정
    
    if(connect(sock, (struct sockaddr *)&addr, sizeof(addr))==-1){//소켓 연결에 실패할 경우
        perror("connect");
        exit(1);
    }
    
    strcat(buf,argv[1]);
    for(int i=2; i<argc; i++){//인자로 들어온 문자열을 한 문자열로 합친다
        strcat(buf," ");
        strcat(buf,argv[i]);
    }
    
    if(send(sock,buf,sizeof(buf),0)==-1){//전송에 실패할 시
        perror("sned");
        exit(1);
    }

    if((strLen=recv(sock,buf,sizeof(buf),0))==-1){//수신에 실패할 시
        perror("recv");
        exit(1);
    }
    buf[strLen]='\0';
    
    printf("From Server : %s\n",buf);

    close(sock);
    return 0;
}
