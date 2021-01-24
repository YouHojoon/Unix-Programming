#include <sys/types.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <time.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <dirent.h>
#include <pwd.h>
#include <grp.h>

const char *HOME_DIR; //홈 디렉토리 경로
char *output = {"%G. %m. %d. (%a) %T "};
time_t t;
struct tm *tm;

void logging(char *buf, int strLen);

int main(int argc, char *argv[])
{
  HOME_DIR = getenv("HOME");             //홈 디렉토리 경로
  const char *ADMIN_PASSWD = "secret\n"; //관리자 비밀번호
  int clntSock, srvSock, strLen, help_fd;
  char buf[BUFSIZ];
  struct sockaddr_in srvAddr, clntAddr;
  socklen_t szAddr;
  pid_t pid;
  DIR *dp;

  if (argc != 2)
  { //소켓을 위한 포트가 인자로 주어지지 않을 시
    printf("Usage %s <PORT>\n", argv[0]);
    exit(1);
  }

  if ((help_fd = open("help.txt", O_RDONLY)) == -1)
  {
    perror("open() help.txt");
    exit(2);
  }

  chdir(HOME_DIR); //홈 디렉토리로 작업 디렉토리를 변경한다

  time(&t);           //현재 시간을 초로 받아옴
  tm = localtime(&t); //시간 정보를 지역 시간대를 기준으로 쪼갬
  tzset();            //지역 시간대로 설정

  if ((srvSock = socket(PF_INET, SOCK_STREAM, 0)) == -1)
  { //소켓을 IPv4 인터넷 프로토콜, TCP소켓으로 생성하는 데 오류가 생겼을 시
    perror("socket()");
    exit(3);
  }
  int bufSize = 0;
  setsockopt(srvSock, SOL_SOCKET, SO_SNDBUF, &bufSize, sizeof(bufSize));
  memset(&srvAddr, 0, sizeof(srvAddr));        //srvAddr을 0으로 초기화
  srvAddr.sin_family = AF_INET;                //IPv4 주소체계를 사용
  srvAddr.sin_addr.s_addr = htonl(INADDR_ANY); //IP주소 자동 할당
  srvAddr.sin_port = htons(atoi(argv[1]));     //인자로 들어온 포트를 정수로 변환하여 지정

  if (bind(srvSock, (struct sockaddr *)&srvAddr, sizeof(srvAddr)) == -1)
  { //소켓을 소켓 정보로 바인딩하는데 오류가 생겼을 시
    perror("bind()");
    exit(4);
  }

  if (listen(srvSock, 5) == -1)
  { //소켓을 클라이언트 요청 대기 상태로 만드는 데 오류가 생겼을 시
    perror("listen()");
    exit(5);
  }

  printf("연결 대기 중\n");

  while (1)
  {
    szAddr = sizeof(clntAddr);

    clntSock = accept(srvSock, (struct sockaddr *)&clntAddr, &szAddr); //클라이언트 연결 수
    if (clntSock == -1)
      continue;

    else
      printf("New Clent--%s\n", inet_ntoa(clntAddr.sin_addr)); //새 클라이언트의 IP출력

    pid = fork();

    if (pid == -1)
    { //자식 프로세스를 만드는 데 오류가 생겼을 시
      close(clntSock);
      continue;
    }

    else if (pid == 0)
    {
      read(clntSock, buf, sizeof(buf));

      if (strcmp(ADMIN_PASSWD, buf) != 0)
      { //지정한 관리자 비밀번호와 일치하지 않을 시
        memset(buf, 0, sizeof(buf));

        sprintf(buf, "관리자 비밀번호가 맞지 않습니다.--%s\n", inet_ntoa(clntAddr.sin_addr));
        logging(buf, strlen(buf));
        printf("%s", buf);

        memset(buf, 0, sizeof(buf));
        strcpy(buf, "fail");

        write(clntSock, buf, strlen(buf));
        close(clntSock);
        exit(0);
      }

      /*
                관리자 로그인 성공 시 현재 날짜와 시간을 출력하며 로그인 성공을 알림
                EX) 2020. 10. 30. (Fri) 23:56:10 KST 접속 성공

            */

      memset(buf, 0, sizeof(buf)); //버퍼 초기화
      strftime(buf, sizeof(buf), output, tm);
      strcat(buf, tzname[0]);
      strcat(buf, " 접속 성공");
      printf("%s--%s\n", buf, inet_ntoa(clntAddr.sin_addr));
      strcat(buf, "\n");
      write(clntSock, buf, strlen(buf));

      memset(buf, 0, sizeof(buf));
      sprintf(buf, "%s 접속 성공\n", inet_ntoa(clntAddr.sin_addr));
      logging(buf, strlen(buf));

      while (1)
      {
        char cmd[100];
        strLen = read(clntSock, buf, sizeof(buf));
        buf[strLen] = '\0';
        logging(buf, strLen);

        if (strLen <= 100)
          strcpy(cmd, buf);
        else
          strcpy(cmd, "");

        memset(buf, 0, sizeof(buf));
        if (strcmp(cmd, "help\n") == 0)
        {
          /*
                    help.txt에 적혀있는 도움말을 읽어서 클라이언트에 보낸다
                */
          while ((strLen = read(help_fd, buf, sizeof(buf))) != 0)
            write(clntSock, buf, strLen);
          lseek(help_fd, 0, SEEK_SET); //파일을 끝까지 다 읽고나서 파일의 오프셋을 맨 처음으로 되돌린다
          continue;
        }

        else if (strcmp(cmd, "ls\n") == 0)
        {
          /*
                    ls -l의 명령어와 같은 결과를 클라이언트에게 보낸다
                  */
          struct dirent *dent;
          struct stat s;
          struct passwd *pw;
          struct group *grp;
          int kind;
          char mod[20];

          if ((dp = opendir(getcwd(NULL, BUFSIZ))) == NULL) //현재 작업 디렉터리를 여는 데 오류가 생길 경우
          {
            perror("opendir()");
            exit(2);
          }

          while ((dent = readdir(dp))) //작업 디렉터리의 내용을 하나씩 받아온다
          {
            if (dent->d_name[0] == '.')
              continue;

            sprintf(buf, "%s/%s", getcwd(NULL, BUFSIZ - strlen(dent->d_name)), dent->d_name);
            stat(buf, &s); //경로를 설정해서 상세 정보를 받아온다
            kind = s.st_mode & S_IFMT;

            switch (kind) //파일 종류 확인
            {
            case S_IFDIR:
              strcpy(mod, "d");
              break;
            case S_IFLNK:
              strcpy(mod, "l");
              break;
            default:
              strcpy(mod, "-");
              break;
            }

            /*
                        파일의 소유자 권한 확인
                      */

            if (s.st_mode & S_IREAD)
              strcat(mod, "r");
            else
              strcat(mod, "-");

            if (s.st_mode & S_IWRITE)
              strcat(mod, "w");
            else
              strcat(mod, "-");

            if (s.st_mode & S_IEXEC)
              strcat(mod, "x");
            else
              strcat(mod, "-");

            /*
                        파일의 그룹 권한 확인
                      */

            if (s.st_mode & (S_IREAD >> 3))
              strcat(mod, "r");
            else
              strcat(mod, "-");

            if (s.st_mode & (S_IWRITE >> 3))
              strcat(mod, "w");
            else
              strcat(mod, "-");

            if (s.st_mode & (S_IEXEC >> 3))
              strcat(mod, "x");
            else
              strcat(mod, "-");

            /*
                        파일의 기타 사용자 권한 확인
                      */

            if (s.st_mode & (S_IREAD >> 6))
              strcat(mod, "r");
            else
              strcat(mod, "-");

            if (s.st_mode & (S_IWRITE >> 6))
              strcat(mod, "w");
            else
              strcat(mod, "-");

            if (s.st_mode & (S_IEXEC >> 6))
              strcat(mod, "x");
            else
              strcat(mod, "-");

            pw = getpwuid(s.st_uid);  // /etc/passwd 파일을 uid로 읽기
            grp = getgrgid(s.st_gid); // /etc/group 파일을 gid로 읽기
            tm = localtime(&s.st_mtim.tv_sec);

            memset(buf, 0, sizeof(buf));
            sprintf(buf, "%s %s %s %d %2d월 %2d:%2d %s\n", mod, pw->pw_name, grp->gr_name, (int)s.st_size, tm->tm_mon + 1, tm->tm_hour, tm->tm_min, dent->d_name);
            write(clntSock, buf, strlen(buf));
          }
          rewinddir(dp);
          continue;
        }
        else if (strncmp(cmd, "cd", 2) == 0 && cmd[2] == ' ')
        {
          /*
                 현재 작업 디렉터리를 옮긴다
                */
          char *path = strtok(cmd, " "); //공백을 기준으로 문자열을 나눈다
          path = strtok(NULL, " ");      //처음 나누면 cd라서 한번 더 나눈다
          path = strtok(path, "\n");     //마지막 개행문자를 지우기 위해

          if (chdir(path) == 0) //작업 디렉토리 이동 성공
            sprintf(buf, "pwd=%s\n", getcwd(NULL, BUFSIZ));

          else
            sprintf(buf, "%s을(를) 찾을 수 없거나 디렉토리가 아닙니다.\n", path);

          write(clntSock, buf, strlen(buf));
          continue;
        }
        else if (strncmp(cmd, "cat", 3) == 0 && cmd[3] == ' ')
        {
          /*
                  파일을 읽어서 클라이언트에게 보내준다
                */
          int fd;
          char *filename = strtok(cmd, " "); //공백을 기준으로 문자열을 나눈다
          filename = strtok(NULL, " ");      //처음 나누면 cat이라서 한번 더 나눈다
          filename = strtok(filename, "\n"); //마지막 개행문자를 지우기 위해

          if ((fd = open(filename, O_RDONLY)) == -1)
          { //파일을 찾을 수 없을 시
            sprintf(buf, "%s을(를) 찾을 수 없습니다\n", filename);
            write(clntSock, buf, strlen(buf));
            continue;
          }

          while ((strLen = read(fd, buf, sizeof(buf))) != 0) //파일 끝까지 읽어서 보냄
            write(clntSock, buf, strLen);

          continue;
        }
        else if (strncmp(cmd, "chmod", 5) == 0 && cmd[5] == ' ' && cmd[9] == ' ')
        {
          /*
                  파일의 권한을 변경한다
                */
          char *filename = strtok(cmd, " ");
          char *mod_str = strtok(NULL, " ");
          int mod = atoi(mod_str); //두번째 인자를 받아 정수형으로 변경
          int cnt = 0;
          mode_t m;

          filename = strtok(NULL, " ");
          filename = strtok(filename, "\n");

          if (mod_str[0] < '0' || mod_str[0] > '7' || mod_str[1] < '0' || mod_str[1] > '7' || mod_str[2] < '0' || mod_str[2] > '7')
          { //권한이 올바르게 입력을 못 받았을 시
            strcpy(buf, "chmod <MOD(숫자)> <FILENAME>로 사용해주세요.\n");
            write(clntSock, buf, strlen(buf));
            continue;
          }

          while (mod != 0)
          {
            int n = mod % 10;
            /*
                    반복한 횟수만큼 권한을 left-shift 연산을 하여서 기타 사용자, 그룹, 소유자의 권한을 설정한
                  */
            if (n >= 4)
            {
              m |= (S_IROTH << 3 * cnt);
              n -= 4;
            }

            if (n >= 2)
            {
              m |= (S_IWOTH << 3 * cnt);
              n -= 2;
            }

            if (n >= 1)
            {
              m |= (S_IXOTH << 3 * cnt);
              n -= 1;
            }
            mod /= 10;
            cnt++;
          }

          if (chmod(filename, m) == -1) //권한 변경에 실패하였을 시
            sprintf(buf, "%s을(를) 찾을 수 없습니다\n", filename);

          else
            strcpy(buf, "권한을 성공적으로 변경하였습니다.\n");

          write(clntSock, buf, strlen(buf));
        }
        else if (strncmp(cmd, "./", 2) == 0)
        {
          /*
                  system() 함수를 통해서 현재 작업 디렉토리에 있는 명령어를 백그라운드로 실행합니다.
                  nohup으로 터미널이 꺼지더라도 실행을 유지시킵니다.
                */
          sprintf(buf, "nohup %s &", strtok(cmd, "\n"));

          memset(buf, 0, sizeof(buf));
          if (system(buf) == -1) //자식 프로세스 생성에 실패하였을 시
            strcpy(buf, "명령어 수행에 실패하였습니다.\n");
          else //잘못된 명령어가 들어와도 system() 함수를 통하여 알아낼 수 없다.
            strcpy(buf, "명령어 수행에 성공하였습니다.(주의 : 명령어가 올바르지 않아도 성공했다고 나옵니다.)\n");

          write(clntSock, buf, strlen(buf));
        }
        else if (strcmp(cmd, "pwd\n") == 0)
        {
          /*
            현재 작업 디렉토리를 클라이언트에게 전송한다.
          */
          sprintf(buf, "pwd=%s\n", getcwd(NULL, BUFSIZ));
          write(clntSock, buf, strlen(buf));
        }
        else if (strcmp(cmd, "logout\n") == 0)
        {
          /*
                  소켓을 닫아 클라이언트와 연결을 끊는다.
                */
          printf("Disconnect Clent--%s\n", inet_ntoa(clntAddr.sin_addr)); //새 클라이언트의 IP출력
          strcpy(buf, "로그아웃합니다.\n");
          write(clntSock, buf, strlen(buf));
          break;
        }
        else
        {
          strcpy(buf, "명령어가 올바르지 않습니다.\n");
          write(clntSock, buf, strlen(buf));
        }
      }

      close(clntSock); //자식 프로세스의 소켓도 닫아야 한다
      exit(1);
    }
    else
      close(clntSock);
  }

  close(srvSock);
  return 0;
}

void logging(char *buf, int strLen)
{
  int fd;
  char date[256];
  char filepath[256];
  mode_t mode = S_IRWXU | S_IRGRP | S_IROTH;

  sprintf(filepath, "%s/log", HOME_DIR);

  if (opendir(filepath) == NULL) //홈 디렉토리 밑에 log디렉토리가 없을 경우 생성
    mkdir(filepath, 0755);

  strcat(filepath, "/log-");

  time(&t);
  tm = localtime(&t);

  /*
        log파일명을 log-2020-10-31.log로 만들기 위해
    */
  strftime(date, sizeof(date), "%F", tm);
  strcat(filepath, date);
  strcat(filepath, ".log");
  fd = open(filepath, O_CREAT | O_WRONLY | O_APPEND, mode);

  if (fd == -1)
  {
    perror("open() log");
    exit(2);
  }

  /*
        로그를 명령어가 입력으로 들어온 날짜와 시간 등 시간 정보와 명령어로 남긴다.
    */
  memset(date, 0, sizeof(date));
  strftime(date, sizeof(date), output, tm);
  strcat(date, " ");
  write(fd, date, strlen(date));
  write(fd, buf, strLen);

  close(fd);
}
