#include <sys/types.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <string.h>

int main(int argc, char* argv[]){
    int fd;
    pid_t pid;
    caddr_t addr;
    struct stat statbuf;

    if((fd=open("test2.txt", O_RDWR | O_CREAT), 664)==-1){//파일을 읽기 쓰기로 여는데 실패할 시
        perror("open");
        exit(1);
    }

    if(ftruncate(fd, (off_t)sysconf(_SC_PAGESIZE)) ==-1){//파일의 크기를 페이지 크기만큼 증가시키는데 실패할 시
        perror("ftruncate");
        exit(1);
    }

    if(fstat(fd,&statbuf) ==-1){//파일의 정보를 받아오는 데 실패할 시
        perror("stat");
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
            strcpy(addr, "Child Writes String");
            break;
        default:
            wait(NULL);

            for(int i=0; i<strlen(addr); i++){
                if(addr[i] > 'a' && addr[i] <'z')//소문자일 경우
                    addr[i]-=32;//32만큼 빼서 대문자로 바꾼다
            }

            printf("%s\n",addr);
            break;
    }
    
    if(munmap(addr, statbuf.st_size) ==-1){//매모리의 매핑을 해제하는데 실패할 경우
        perror("munmap");
        exit(1);
    }

    return 0;

}
