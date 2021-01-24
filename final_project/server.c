#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <glib.h>
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

#define WORD_LENGTH 12 //낱말 길이
#define ROOM_LIST_SIZE 5 //방 개수
#define ROOM_SIZE 5//방의 유저 제한
#define TIME_LIMIT 15//끝말잇기 게임에서의 시간 제한

typedef struct user
{
    int sock;
    int isMaster;//방장여부
    int room_num;//방 번호
    char name[10];
} user;

typedef struct room{
    GList *userList;//방의 유저 목록
    pthread_mutex_t lock;
    int isStart;//게임 시작 여부
    int isFull;//유저 제한 까지 유저가 들어 왔는지 여부
    GList* t_idList;//방의 스레드 목록
    user* turn_user;//게임에서 순서인 유저
    pthread_t game_t_id;//게임의 스레드
    pthread_t adj_t_id;//단어 검사 스레드
    pthread_t adv_t_id;//단어 검사 스레드
    pthread_t noun_t_id;//단어 검사 스레드
    pthread_t sense_t_id;//단어 검사 스레드
    pthread_t verb_t_id;//단어 검사 스레드
    int isFirst;//현재 유저가 첫 번째인지 여부
}room;

typedef struct check_word{
    int room_num;
    char word[WORD_LENGTH];
}check_word;

room *roomList[ROOM_LIST_SIZE];

void send_msg(char *msg, int len, int sock, int room_num);
void *chat(void *arg);
void *connect_clnt(void *arg);
void *game(void* arg);
void disconnect(int sock);
void *time_limit_alarm();
void *adj(void* arg);
void *adv(void* arg);
void *noun(void* arg);
void *sense(void* arg);
void *verb(void* arg);

int main(int argc, char *argv[])
{
    int srvSock, clntSock;
    struct sockaddr_in srvAddr, clntAddr;
    socklen_t szAddr;
    pthread_t t_id;

    if (argc != 2)
    {
        printf("Usage %s <PORT>\n", argv[0]);
        exit(1);
    }

    if ((srvSock = socket(PF_INET, SOCK_STREAM, 0)) == -1)//소켓을 IPv4 TCP소켓으로 만드는데 실패할 경우
    {
        perror("socket");
        exit(1);
    }
     
    for(int i=0; i<ROOM_LIST_SIZE; i++){
       /*
            방 목록 초기화
        */
       room *r= (room *)malloc(sizeof(room));
       pthread_mutex_init(&(r->lock),NULL);
       r->isStart=0;
       r->isFull = 0;
       roomList[i]=r;
       r->userList = NULL;
    }

    memset(&srvAddr, 0, sizeof(srvAddr));
    srvAddr.sin_family = AF_INET;//IPv4 설정
    srvAddr.sin_addr.s_addr = htonl(INADDR_ANY);//소켓 IP설정
    srvAddr.sin_port = htons(atoi(argv[1]));//소켓 포트 설정

    if (bind(srvSock, (struct sockaddr *)&srvAddr, sizeof(srvAddr)) == -1)
    {
        perror("bind");
        exit(1);
    }

    if (listen(srvSock, 5) == -1)
    {
        perror("listen");
        exit(1);
    }

    while (1)
    {
        szAddr = sizeof(clntAddr);
        if ((clntSock = accept(srvSock, (struct sockaddr *)&clntAddr, &szAddr)) == -1)
            continue;
         
        char *buff = (char *)malloc(255*sizeof(char));
        strcpy(buff,"---방 목록---\n");
        for(int i=0; i<ROOM_LIST_SIZE;i++){
            char *str=(char *)malloc(50*sizeof(char));
            room *r = roomList[i];
            if(!r->isFull && !r->isStart){//빈 자리가 있는 게임을 시작하지 않은 방 목록 출력
                sprintf(str, "%d번방[%d/%d]\n", i+1, g_list_length(r->userList),ROOM_SIZE);
                strcat(buff,str);
            }
            free(str);
        }
        write(clntSock,buff,strlen(buff));

        int strLen = read(clntSock,buff,sizeof(buff));
        buff[strLen-1]='\0';
        int room_num = atoi(buff)-1; 

        if(room_num<0||room_num>=ROOM_LIST_SIZE || 
                roomList[room_num]->isFull || roomList[room_num]->isStart){
            /*
                올바르지 않은 방 번호를 받았을 시
             */
            write(clntSock,"0\n",2);
            close(clntSock);
            continue;
        }

        else
            write(clntSock,"1\n",2);

        user *u = (user *)malloc(sizeof(user));
        u->sock = clntSock;
        u->room_num = atoi(buff)-1;

        free(buff);
        pthread_create(&t_id, NULL, connect_clnt, (void *)u);//클라이언트 연결 스레드 생성
        pthread_detach(t_id);//스레드 분리
        printf("Connected client IP: %s \n", inet_ntoa(clntAddr.sin_addr));//연결된 클라이언트 IP 출력
    }

    close(srvSock);
    return 0;
}
void *connect_clnt(void *arg)   
{
    /*
        클라이언트 연결 스레드
     */
    user *u = (user *)arg;
    pthread_t t_id;
    int room_num = u->room_num;
    pthread_mutex_lock(&(roomList[room_num]->lock));
    
    int user_count = g_list_length(roomList[room_num]->userList);

    if (user_count == 0)//만약 첫 번째로 입장한 유저일 경우
        u->isMaster = 1;
    else if(user_count == ROOM_SIZE-1){//만약 마지막으로 입장한 유저일 경우
        roomList[room_num]->isFull=1;
        u->isMaster=0;
    }
    else
        u->isMaster = 0;

    char *buff = (char *)malloc(sizeof(char) * 255);
    int strLen = read(u->sock, buff, sizeof(buff));
    buff[strLen] = '\0';
    GList *i=roomList[room_num]->userList;
    
    do{      
        user *tmp;

        if(i==NULL)
            tmp = NULL;
        
        else
            tmp=(user *)(i->data);
        
        if(strcmp(buff,"!exit")==0){//연결 종료
            free(buff);
            close(u->sock);
            return NULL;
        }
        
        if(tmp!=NULL)           
            if(strcmp(tmp->name,buff)==0){//닉네임이 이미 존재할 경우
                write(u->sock,"0\n",2);
                strLen = read(u->sock, buff, sizeof(buff));
                buff[strLen] = '\0'; // \n문자 제거
                i=roomList[room_num]->userList;
            }
        else
            i=i->next;
    }while(i!=NULL);
    
    write(u->sock,"1\n",2);
    strcpy(u->name, buff);
    roomList[room_num]->userList = g_list_insert(roomList[room_num]->userList, u, -1);
    pthread_mutex_unlock(&(roomList[room_num]->lock));
    
    if(u->isMaster==0)
        sprintf(buff, "환영합니다! [%s]님 방장의 게임 시작을 기다려주세요!(퇴장 : !exit)\n",u->name);
    else
        sprintf(buff, "환영합니다! [%s]님 게임 시작은 !start로 해주세요!(퇴장 : !exit)\n",u->name);
    
    sleep(1);
    write(u->sock,buff,strlen(buff));
    free(buff);

    pthread_create(&t_id, NULL, chat, (void *)u);//채팅 스레드 생성
    pthread_detach(t_id);//스레드 분리
}
void send_msg(char *msg, int len, int sock, int room_num)
{
    /*
        메시지 전송 함수
        sock이 음수라면 모든 유저에게 전송하고
        양수라면 입력받은 소켓은 제외하여 전송
     */
    pthread_mutex_lock(&(roomList[room_num]->lock)); 
   
    for (GList *i = roomList[room_num]->userList; i != NULL; i = i->next)
    {
        user *u = (user *)(i->data);
        if (sock>=0 && u->sock == sock)
            continue;

        write(u->sock, msg, len);
        
    }
    pthread_mutex_unlock(&(roomList[room_num]->lock)); 
}
void *chat(void *arg)
{
    /*
      채팅 스레드
     */
    user *u = (user *)arg;
    int strLen, room_num=u->room_num;
    char msg[255];
    char buff[BUFSIZ];
    pthread_t t_id;

    room* r = roomList[room_num];
    r->t_idList=g_list_insert(r->t_idList,GINT_TO_POINTER(pthread_self()),-1);

    while ((strLen = read(u->sock, msg, sizeof(msg))) != 0)
    {
        msg[strLen] = '\0';
        if(strcmp(msg,"!start\n")==0 && u->isMaster==1){
        /*
            끝말잇기 게임 시작
         */
            roomList[room_num]->isStart=1;
            send_msg("5\n",2,-1, room_num);
            sleep(1);
            send_msg("4\n",2,-1, room_num);
            sleep(1);
            send_msg("3\n",2,-1, room_num);
            sleep(1);
            send_msg("2\n",2,-1, room_num);
            sleep(1);
            send_msg("1\n",2,-1, room_num);
            sleep(1);
            send_msg("0\n",2,-1, room_num);
            sleep(1);
           
            for(GList *i=r->t_idList; i!=NULL; i=i->next){//채팅 스레드 종료
                pthread_t tmp=*(pthread_t *)(i->data);
                if(tmp!=pthread_self()){
                   pthread_cancel(tmp);
                   r->t_idList=g_list_remove_link(r->t_idList,i);
                }
            }

            pthread_create(&t_id,NULL, game, (void *)&room_num);          
            pthread_join(t_id,NULL);
            //끝말잇기 게임 종료 후
            for(GList *i=r->userList; i!=NULL; i=i->next){
                user *tmp=(user *)(i->data);
                if(tmp!=u){
                   pthread_create(&t_id,NULL,chat,(void*)tmp);//채팅 스레드 재생성
                   pthread_detach(t_id);
                }
            }

            r->isStart=0;
            continue;
        }
        else if(strcmp(msg,"!exit\n")==0){//방 퇴장
            pthread_mutex_lock(&r->lock);
            r->userList=g_list_remove(r->userList,u);//유저 제거
            sprintf(buff, "[%s]이 퇴장하셨습니다\n", u->name);
            
            if(r->isFull)//만약 가득 찬 방일 경우
                r->isFull = 0;

            if(u->isMaster){//유저가 방장일 경우
               if(r->userList!=NULL){//유저 이외 다른 유저가 존재할 경우
                   user *tmp = (user *)(r->userList->data);
                   char *master_change_msg = (char *)malloc(100*sizeof(char));
                   tmp->isMaster=1;
                   sprintf(master_change_msg, "[%s]이 방장이 되었습니다. !start로 게임을 시작해주세요\n", tmp->name);
                   strcat(buff,master_change_msg);
                   free(master_change_msg);
               }
            }
            
            pthread_mutex_unlock(&r->lock);
            send_msg(buff, strlen(buff), -1, room_num);
            close(u->sock);
           
            return NULL;
        }

        sprintf(buff, "[%s] : %s", u->name, msg);
        send_msg(buff, strlen(buff), u->sock, room_num);
    }

    return NULL;
}
void *game(void* arg){
    /*
        끝말잇기 게임
     */
    int room_num = *(int *)arg;
    room *r = roomList[room_num];
    GList *wordList,*i=roomList[room_num]->userList;
    char buff[BUFSIZ], word[WORD_LENGTH];
    user *u;
    int  strLen, adj_result=0, adv_result=0, noun_result=0, sense_result=0, verb_result=0;
    pthread_t t_id;
    roomList[room_num]->game_t_id=pthread_self();
    check_word *cw=(check_word *)malloc(sizeof(check_word));
    cw->room_num = room_num;
    r->isFirst=1;

    while(1){
        u = (user *)(i->data);
        roomList[room_num]->turn_user = u;
        sprintf(buff,"[%s]님 차례입니다.\n",u->name);
        send_msg(buff,strlen(buff),-1, room_num);
        
        pthread_create(&t_id,NULL,time_limit_alarm,(void *)&room_num);//시간제한 측정 스레드 생성
        pthread_detach(t_id);//스레드 분리
        
        strLen = read(u->sock,word,sizeof(word));
        word[strLen-1] = '\0';
        strcpy(cw->word,word);
        pthread_cancel(t_id);//read함수가 반환할 경우 시간 내에 입력을 받았다는 것이므로 시간제한 측정 스레드 취소
        
        if(strlen(word)-1<2){//두 글자 이하일 시
            sprintf(buff,"단어를 두 글자 이상 입력해주세요.\n");
            send_msg(buff,strlen(buff),-1,room_num);
            pthread_cancel(t_id);
            continue;
        }
        /*
            단어 검사 스레드
         */
        pthread_create(&r->adj_t_id,NULL,adj,(void *)cw);
        pthread_create(&r->adv_t_id,NULL,adj,(void *)cw);
        pthread_create(&r->verb_t_id,NULL,adj,(void *)cw);
        pthread_create(&r->noun_t_id,NULL,adj,(void *)cw);
        pthread_create(&r->sense_t_id,NULL,adj,(void *)cw);
        
        pthread_join(r->adj_t_id,(void *)&adj_result);
        pthread_join(r->adv_t_id,(void *)&adv_result);
        pthread_join(r->noun_t_id,(void *)&noun_result);
        pthread_join(r->verb_t_id,(void *)&verb_result);
        pthread_join(r->sense_t_id,(void *)&sense_result);

        if(!adj_result && !adv_result && !verb_result && !noun_result && !sense_result){
           sprintf(buff,"[%s]님이 패배하셨습니다.\n", u->name);
            send_msg(buff,strlen(buff),-1, room_num);
            break;
        }
    
        if(!r->isFirst){
            char *last_word = (char *)(g_list_last(wordList)->data);
            if(last_word[strlen(last_word)-1]!=word[0]){
            /*
                끝말이 다를 시
             */
               sprintf(buff,"[%s]님이 패배하셨습니다.\n", u->name);
               send_msg(buff,strlen(buff),-1, room_num);
               break;
            }
           
            for(GList *i=wordList; i!=NULL; i=i->next){
                last_word = (char *)(i->data);
                if(strcmp(word,last_word)==0){
                /*
                    이전에 나온 낱말일 시
                 */
                    sprintf(buff,"[%s]님이 패배하셨습니다.\n", u->name);
                    send_msg(buff,strlen(buff),-1, room_num);
                    goto END;//중간 반복문에서 바깥 반복문을 탈출하기 위해 goto사용
                }
            }

        }
        else
            r->isFirst=0;

        char *tmp=(char *)malloc(WORD_LENGTH * sizeof(char));
        strcpy(tmp,word);

        wordList=g_list_insert(wordList,tmp,-1);     
        sprintf(buff,"[%s] : %s\n",u->name, word);
        send_msg(buff,strlen(buff),-1, room_num);
        
        i=i->next;
        if(i==NULL)//순서가 모두 돌았을 경우
            i=roomList[room_num]->userList;
        
    }

END:
    return NULL;
}

void *time_limit_alarm(void* arg){
    int room_num =*(int*)arg;
    char buff[50];
    user* u = roomList[room_num]->turn_user;
    pthread_t t_id = roomList[room_num]->game_t_id;

    sleep(TIME_LIMIT);//TIME_LIMIT에 설정된 시간까지 스레드 휴식
    sprintf(buff,"[%s]님이 패배하셨습니다.\n", u->name);
    send_msg(buff,strlen(buff),-1, room_num);
    pthread_cancel(t_id);//제한시간이 끝낫으므로 끝말잇기 게임 스레드 취소
}

void *adj(void* arg){
    FILE *fp;
    char tmp[BUFSIZ];
    char *word;
    check_word *cw = (check_word *)arg;
    int room_num = cw->room_num;
    room *r = roomList[room_num];

    if((fp=fopen("index.adj","r"))==NULL){
        perror("fopen");
        exit(1);
    }

    while(1){
        if(fgets(tmp,sizeof(tmp),fp)==NULL)
            break;

        word = strtok(tmp, " ");//공백을 기준으로 문자열 분리
        if(strcmp(word,cw->word)==0){
            /*
                단어가 있다면 나머지 스레드 종료
             */
            pthread_cancel(r->adv_t_id);
            pthread_cancel(r->noun_t_id);
            pthread_cancel(r->verb_t_id);
            pthread_cancel(r->sense_t_id);
            
            fclose(fp);
            return (void *)1;
        }
    }

    fclose(fp);
    return (void *)0;

}
void *adv(void* arg){
    FILE *fp;
    char tmp[BUFSIZ];
    char *word;
    check_word *cw = (check_word *)arg;
    int room_num = cw->room_num;
    room *r = roomList[room_num];

    if((fp=fopen("index.adv","r"))==NULL){
        perror("fopen");
        exit(1);
    }

    while(1){
        if(fgets(tmp,sizeof(tmp),fp)==NULL)
            break;

        word = strtok(tmp, " ");//공백을 기준으로 문자열 분리
        if(strcmp(word,cw->word)==0){
            /*
                단어가 있다면 나머지 스레드 종료
             */
            pthread_cancel(r->adj_t_id);
            pthread_cancel(r->noun_t_id);
            pthread_cancel(r->verb_t_id);
            pthread_cancel(r->sense_t_id);
            
            fclose(fp);
            return (void *)1;
        }
    }

    fclose(fp);
    return (void *)0;

}
void *noun(void* arg){
    FILE *fp;
    char tmp[BUFSIZ];
    char *word;
    check_word *cw = (check_word *)arg;
    int room_num = cw->room_num;
    room *r = roomList[room_num];

    if((fp=fopen("index.noun","r"))==NULL){
        perror("fopen");
        exit(1);
    }

    while(1){
        if(fgets(tmp,sizeof(tmp),fp)==NULL)
            break;

        word = strtok(tmp, " ");//공백을 기준으로 문자열 분리
        if(strcmp(word,cw->word)==0){
            /*
                단어가 있다면 나머지 스레드 종료
             */
            pthread_cancel(r->adj_t_id);
            pthread_cancel(r->adv_t_id);
            pthread_cancel(r->verb_t_id);
            pthread_cancel(r->sense_t_id);
            
            fclose(fp);
            return (void *)1;
        }
    }

    fclose(fp);
    return (void *)0;

}
void *verb(void* arg){
    FILE *fp;
    char tmp[BUFSIZ];
    char *word;
    check_word *cw = (check_word *)arg;
    int room_num = cw->room_num;
    room *r = roomList[room_num];

    if((fp=fopen("index.verb","r"))==NULL){
        perror("fopen");
        exit(1);
    }

    while(1){
        if(fgets(tmp,sizeof(tmp),fp)==NULL)
            break;

        word = strtok(tmp, " ");//공백을 기준으로 스레드 분리
        if(strcmp(word,cw->word)==0){
            /*
                단어가 있다면 나머지 스레드 종료
             */
            pthread_cancel(r->adv_t_id);
            pthread_cancel(r->noun_t_id);
            pthread_cancel(r->adj_t_id);
            pthread_cancel(r->sense_t_id);
           
            fclose(fp);
            return (void *)1;
        }
    }

    fclose(fp);
    return (void *)0;

}
void *sense(void* arg){
    FILE *fp;
    char tmp[BUFSIZ];
    char *word;
    check_word *cw = (check_word *)arg;
    int room_num = cw->room_num;
    room *r = roomList[room_num];

    if((fp=fopen("index.sense","r"))==NULL){
        perror("fopen");
        exit(1);
    }

    while(1){
        if(fgets(tmp,sizeof(tmp),fp)==NULL)
            break;

        word = strtok(tmp, " ");//공백을 기준으로 문자열 분리
        if(strcmp(word,cw->word)==0){
            /*
                단어가 있다면 나머지 스레드 종료
             */
            pthread_cancel(r->adv_t_id);
            pthread_cancel(r->noun_t_id);
            pthread_cancel(r->verb_t_id);
            pthread_cancel(r->adj_t_id);
            
            fclose(fp);
            return (void *)1;
        }
    }

    fclose(fp);
    return (void *)0;

}
