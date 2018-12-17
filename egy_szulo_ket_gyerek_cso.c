#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

//https://stackoverflow.com/questions/53807518/can-i-use-pipe-as-read-at-parent-and-write-in-child

int main(){

  int  p1[2], p2[2];
  char original[]="This is a string\n ";

  pid_t child2;
  pid_t child=fork();

  if (child>0)
  {
    child2 = fork();
    if(child2>0){
      pipe(p2);
      pipe(p1);

      wait(NULL);
      printf("I'm the parrent.\n");

      close(p1[1]);
      close(p2[1]);

      printf("Parrent read p1:\n");
      if (read(p1[0], original, sizeof(original)) == -1)
        perror("read() error in parent p1");
      else printf("parent read '%s' from pipe 1\n", original);

      printf("Parrent read p2:\n");
      if (read(p2[0], original, sizeof(original)) == -1)
        perror("read() error in parent p2");
      else printf("parent read '%s' from pipe 2\n", original);
    }
    else{
      printf("Child2 \n");
      //pipe(p2);
      close(p2[0]);
      if (write(p2[1], original, sizeof(original)+1) == -1)
        perror("write() error in child2");
      //close(p2[1]);
    }
  }
  else
  {
    printf("Child1 \n");

    //pipe(p1);
    close(p1[0]);
    if (write(p1[1], original, sizeof(original)+1) == -1)
      perror("write() error in child1");
    //close(p1[1]);
  }
  return 0;
}