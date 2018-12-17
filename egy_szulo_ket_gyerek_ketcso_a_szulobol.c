#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

//https://github.com/angrave/SystemProgramming/wiki/Pipes%2C-Part-2%3A-Pipe-programming-secrets#pipe-gotchas


int main(){

  char original1[]="This is a string for child1";
  char original2[]="This is a string for child2";

  int  p1[2], p2[2];
  pid_t child2;
  pid_t child=fork();

      pipe(p2);
      pipe(p1);

  if (child>0)
  {
    child2 = fork();
    if(child2>0){

      printf("I'm the parent.\n");

      close(p2[0]);
      if (write(p2[1], original2, sizeof(original2)+1) == -1)
        perror("write() error in parent");

      close(p1[0]);
      if (write(p1[1], original1, sizeof(original1)+1) == -1)
        perror("write() error in parent");
      
      wait(NULL);
    }
    else{
      printf("Child2 \n");
      close(p2[1]);

      //printf("Child2 read p2:\n");
      if (read(p2[0], original2, sizeof(original2)+1) == -1)
        perror("read() error in child2 p2");
      else printf("child2 read '%s' from pipe 2\n", original2);
    }
  }
  else
  {
    printf("Child1 \n");
    close(p1[1]);

    //printf("Child1 read p1:\n");
    if (read(p1[0], original1, sizeof(original1)+1) == -1)
      perror("read() error in child1 p1");
    else printf("child1 read '%s' from pipe 1\n", original1);

  }
  return 0;
}

/*

int main() {
  int fd[2];
  pipe(fd);
  // You must read from fd[0] and write from fd[1]
  printf("Reading from %d, writing to %d\n", fd[0], fd[1]);

  pid_t p = fork();
  if (p > 0) {
//      I have a child therefore I am the parent
      write(fd[1],"Hi Child!",9);

      //don't forget your child
      wait(NULL);
  } else {
      char buf;
      int bytesread;
      // read one byte at a time.
      while ((bytesread = read(fd[0], &buf, 1)) > 0) {
          putchar(buf);
          if (buf == '!') break; // End of message 
      }
  }
  return 0;
}*/