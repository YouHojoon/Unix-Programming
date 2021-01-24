#include <sys/types.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main(int argc, char* argv[]){
    int fd;
    pid_t pid;
    caddr_t addr;
    struct stat statbuf;

    if(argc !=2){
        fprintf(stderr,"Useage: %s filename\n", argv[0]);
        exit(1);
    }

    if(stat(argv[1],&statbuf) ==-1){//파일의 정보를 받아오는 데 실패할 시
        perror("stat");
        exit(1);
    }

    if((fd=open(argv[1], O_RDWR))==-1){//파일을 읽기 쓰기로 여는데 실패할 시
        perror("open");
        exit(1);
    }

    addr = mmap(NULL, statbuf.st_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, (off_t)0);//메모리에 파일의 사이즈만큼 매핑을하고 보호모드는 읽기 쓰기 허용, 데이터 변경 내용을 공유하도록 한다

    if(addr == MAP_FAILED){//mmap에 실패할 경우
        perror("mmap");
        exit(1);
    }

    close(fd);

    switch(pid = fork()){
    
        case -1:
            perror("fork");
            exit(1);
            break;
        case 0:
            printf("1. Child Process :addr=%s", addr);
            sleep(1);
            addr[0] = 'C';
            printf("2. Child Process :addr=%s", addr);
            sleep(2);
            printf("3. Child Process :addr=%s", addr);
            break;
        default:
            printf("1. Parent Process :addr=%s", addr);
            sleep(2);
            addr[1] = 'P';
            printf("2. Parent Process :addr=%s", addr);
            sleep(2);
            printf("3. Parent Process :addr=%s", addr);
            break;
    }

    if(munmap(addr, statbuf.st_size) ==-1){//메모리의 매핑을 해제하는데 실패할 경우
        perror("munmap");
        exit(1);
    }

    return 0;

}
