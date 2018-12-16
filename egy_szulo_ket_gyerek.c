#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <sys/types.h>

int main(){
  pid_t child2;
  pid_t child=fork();
  if (child>0)
  {
    child2 = fork();
    if(child2>0){
      wait();
      printf("Szülő vagyok signalt várok\n");
    }
    else{
      printf("Gyerek2 vagyok siganlt küldök\n");
    }
  }
  else
  {
    printf("Gyerek1 vagyok.\n");
  }
  return 0;
}