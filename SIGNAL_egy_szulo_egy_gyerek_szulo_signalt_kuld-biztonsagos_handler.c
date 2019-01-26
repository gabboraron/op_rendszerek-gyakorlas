#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>

#define WRITE(Str) (void)write(1,Str,strlen(Str))
void handler(int signumber)
{
    WRITE("Signal arrived\n");
}

int main(){

  sigset_t sigset;
  sigemptyset(&sigset); //empty signal set
  sigaddset(&sigset,SIGTERM); //SIGTERM is in set
  //sigfillset(&sigset); //each signal is in the set
  sigprocmask(SIG_BLOCK,&sigset,NULL); //signals in sigset will be blocked
               //parameters, how: SIG_BLOCK, SIG_UNBLOCK, SIG_SETMASK -   ;
               //2. parameter changes the signalset to this if it is not NULL,
               //3.parameter if it is not NULL, the formerly used set is stored here
    
  signal(SIGTERM,handler); //signal and handler is connetcted
  //signal(SIGUSR1,handler); 

  pid_t child=fork();
  if(0>child) return EXIT_FAILURE;
  if (child>0) {
    int status;
    printf("I'm the parent\n");
    printf("Waits 2 seconds, then send a SIGTERM %i signal (it is blocked)\n",SIGTERM);
    sleep(2);
    kill(child,SIGTERM);
    printf("I sent it.\n");
  }
  else
  {
    sigemptyset(&sigset); //the child has a different addr-space
                          //so this won't affect the parent
    printf("I'm the child waiting for signal.\n");
    sigsuspend(&sigset);
    printf("Child process ended\n");

  }
  return 0;
}