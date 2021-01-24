#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

#define N 100000000

unsigned int s = 0;
pthread_mutex_t lock;

void *c(void *d){
    int i;
    pthread_mutex_lock(&lock); //mutex lock을 잠근다. 만약 lock이 잠겨있다면 기다린다.
    for (i = 0; i< N; i++)
        s++;
    pthread_mutex_unlock(&lock);//mutex lock의 잠금을 해제한다.
    return NULL;
}

int main(void){
    pthread_t t[2];
    int r;
    
    pthread_mutex_init(&lock,NULL);//mutex lock을 초기화한다

    r = pthread_create(&t[0], NULL, c, NULL);//c함수를 실행하는 스레드를 생성한다
    if(r){//스레드 생성에 실패할 시
        perror("Thread create:");
        exit(1);
    }
    r = pthread_create(&t[1], NULL, c, NULL);
    if(r){
        perror("Thread create:");
        exit(2);
    }
    pthread_join(t[0],NULL);//스레드가 끝날 때 까지 기다린다
    pthread_join(t[1],NULL);
    printf("%u\n",s);

    return 0;
}
