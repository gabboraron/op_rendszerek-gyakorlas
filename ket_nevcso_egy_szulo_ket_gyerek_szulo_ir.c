#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>

int main(){

  char original1[]="This is a string for child1";
  char original2[]="This is a string for child2";


    int pid,fd,fd2;
    printf("Fifo start!\n");
    int fid=mkfifo("fifo.ftc", S_IRUSR|S_IWUSR ); // creating named pipe file
    int fid2=mkfifo("fifo2.ftc", S_IRUSR|S_IWUSR ); // creating named pipe file

    if (fid==-1)  //error handling
    {
      printf("Error at fid number: %i",errno);
      exit(EXIT_FAILURE);
    }
    if (fid2==-1)  //error handling
    {
      printf("Error at fid2 number: %i",errno);
      exit(EXIT_FAILURE);
    }
    printf("Mkfifo vege, fork indul!\n");

  pid_t child2;
  pid_t child=fork();

  if (child>0)
  {
    child2 = fork();
    if(child2>0){

    printf("Szülő vagyok, irok a csobe!\n");
    printf("Csonyitas eredmenye: %d!\n",fid);
          fd=open("fifo.ftc",O_WRONLY);
          write(fd,"Hajra Fradi!\n",12);
          close(fd);
    printf("Szülő vagyok, beirtam, az 1.-t!\n");


    printf("Szülő vagyok, irok a 2. csobe!\n");
    printf("Csonyitas eredmenye: %d!\n",fid2);
          fd2=open("fifo2.ftc",O_WRONLY);
          write(fd2,"Hajra VASAS!\n",12);
          close(fd2);
    printf("Szülő vagyok, beirtam a 2.at, vegeztem!\n");

    wait(NULL);
    }
    else{
      printf("Child2 \n");
      char s[1024]="Semmi";  
     
      printf("Csonyitas eredmenye gyerek2-ben: %d!\n",fid);
      fd=open("fifo.ftc",O_RDONLY);
      read(fd,s,sizeof(s));
      printf("Ezt olvastam a fd csobol: %s gyerek2-ben\n",s);
      close(fd);
      // remove fifo.ftc
      unlink("fifo.ftc");
    }
  }
  else
  {
    char s[1024]="Semmi";
    printf("Child1 \n");

    printf("Csonyitas eredmenye gyerek1-ben: %d!\n",fid2);
    fd2=open("fifo2.ftc",O_RDONLY);
    read(fd2,s,sizeof(s));
    printf("Ezt olvastam az fd2 csobol: %s gyerek1-ben \n",s);
    close(fd2);
    // remove fifo2.ftc
    unlink("fifo2.ftc");

  }
  return 0;
}