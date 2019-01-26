# Info
Egy jó leírás, elmélet + gyakorlati: https://github.com/angrave/SystemProgramming/wiki
> **Ez csak egy összefoglaló!**
Futó processek megtekintése: `top -U [username]` . Bővebben: https://unix.stackexchange.com/questions/85466/how-to-see-process-created-by-specific-user-in-unix-linux 

# Szülő-gyerek kapcsolatok
A szülő-gyerek kapcsolatban azt, hogy melyik fut le hamarabb azt egy `wait` szabályozza, így eben az esetben a szülő:
```` C
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <sys/types.h>

int main(){
  pid_t child=fork();
  if (child>0)
  {
    printf("Szülő vagyok, én futok hamarabb!\n");
  }
  else{
    wait();
    printf("Gyerek vagyok, szülőt várok! \n");
  }
  return 0;
}
````

Míg itt a gyerek:
```` C
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <sys/types.h>

int main(){
  pid_t child=fork();
  if (child>0)
  {
    wait();
    printf("Szülő vagyok, gyereket megvárom!\n");
  }
  else{
    printf("Gyerek vagyok \n");
  }
  return 0;
}
````
> Egy fork, egy gyerek!

# Csövek (pipe)
## Névtelen cső

Csövet a 
````C
int fd[2];
pipe(fd);
````
utasítások hoznak létre.

> Itt a **szülő ír** a cső egyik végébe(`fd[1]`), majd a **gyerek kiolvassa** azt a másik végén `fd[0]`. Ezzel a módszerrel az **ismert hosszúságú** bejegyzést tudjuk kiolvasni a csőből.
````C
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
  int fd[2];
  pipe(fd);
  // You must read from fd[0] and write from fd[1]
  printf("INFO:\tReading from %d, writing to %d\n", fd[0], fd[1]);

  pid_t p = fork();
  if (p > 0) {
      /* I have a child therefore I am the parent*/
      printf("PARR:\tI'm the parrent!\n");
      write(fd[1],"Hi Child!",9);

      /*don't forget your child*/
  } else {
      char buf;
      
      wait(NULL);
      printf("CHILD:\tI'm the child!\n");
      
      read(fd[0], &buf, 9);
      printf("buf: %s\n", &buf);
  }
  return 0;
}
````
Kiolvashatunk **ismeretlen hosszúsgút** is, pl így:
````C
// read one byte at a time => read unknown length 'string'
while ((bytesread = read(fd[0], &buf, 1)) > 0) {
  putchar(buf);
    if (buf == '!') break; // End of message
}
````
egyben:
````C
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
  int fd[2];
  pipe(fd);
  // You must read from fd[0] and write from fd[1]
  printf("INFO:\tReading from %d, writing to %d\n", fd[0], fd[1]);

  pid_t p = fork();
  if (p > 0) {
      /* I have a child therefore I am the parent*/
      printf("PARR:\tI'm the parrent!\n");
      write(fd[1],"Hi Child!",9);

      /*don't forget your child*/
      //wait(NULL);
  } else {
      char buf;
      int bytesread;
      
      wait(NULL);
      printf("CHILD:\tI'm the child!\n");
      // read one byte at a time => read unknown length 'string'
      while ((bytesread = read(fd[0], &buf, 1)) > 0) {
          putchar(buf);
          if (buf == '!') break; // End of message
      }
  }
  return 0;
}
````

> Illetve a **szülő is olvashat**:
````C
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
  int fd[2];
  pipe(fd);
  // You must read from fd[0] and write from fd[1]
  printf("INFO:\tReading from %d, writing to %d\n", fd[0], fd[1]);

  pid_t p = fork();
  if (p > 0) {
      char buf;
      int bytesread;
      /* I have a child therefore I am the parent*/
      wait(NULL);
      printf("PARR:\tI'm the parrent!\n");
      
      // read one byte at a time => read unknown length 'string'
      /*while ((bytesread = read(fd[0], &buf, 1)) > 0) {
          putchar(buf);
          if (buf == '!') break; // End of message
      }*/

      //read knowed length 'string'
      read(fd[0], &buf, 9);
      printf("buf: %s\n", &buf);

      /*don't forget your child*/      
  } else {      
      printf("CHILD:\tI'm the child!\n");
      write(fd[1],"Hi mom!",9);
  }
  return 0;
}
````

## Nevesített csövek (névcső)

# Jelzés (signal)

Ebben a példában **egy szülő** és **egy gyerek** van ahol a **gyerek küld** signalt.
> fájl: SIGNAL_egy_szulo_egy_gyerek_gyerek_signalt_küld.c

````C
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <sys/types.h>

void handler(int signumber){
  printf("Signal with number %i has arrived\n",signumber);
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
  if (child>0)
  {
      wait();
      printf("Szülő vagyok signalt várok\n");

      sigprocmask(SIG_UNBLOCK,&sigset,NULL); //ha az unblock kimarad akkor nem biztosított a megérkezés sorrendje! 
      int status;
      wait(&status);
      printf("Parent process ended\n");
  }
  else
  {
    printf("Gyerek vagyok.\n");
    printf("Waits 2 seconds, then send a SIGTERM %i signal (it is blocked)\n",SIGTERM);
    sleep(2);
    kill(getppid(),SIGTERM);
  }
  return 0;
}
````

> A `sigprocmask`-nak fontos szerepe van, ugyanis ha kihagyjuk akkor *szabályozhatatlanná válik a sorrend*, itt pl a ` printf("Szülő vagyok signalt várok\n");` hamarabb *futhat* le abban az esetben!

> Signalok esetében a *handler*ben `printf`et használni nem egéyszéges! Bővebben stackowerflown: https://stackoverflow.com/a/16507805 vagy https://stackoverflow.com/a/9547988 vagy a dokumentációban: http://man7.org/linux/man-pages/man7/signal.7.html 

**A `signal()`-t a `fork()` előtt kell meghívni, vagy mindenképp gondoskodni kell a szinkronizációról!**
> A szinkronizációról pl így lehet gondoskodni: http://www.code2learn.com/2011/01/signal-program-using-parent-child.html vagy így: https://stackoverflow.com/a/31102010

> **Figyelem!** Ezekben az esetekben a `return code`okat is figyelni kell, ugyanis azok a függvények és eljárások amik természetes esetben nem hasalhatnak el, ilyen esetekben elhasalhatnak, ha kigofynak pl memóriából, ilyen pl a `fork()` is!

Az alábbi példában egy **szülő küld** a **gyereknek** jelzést:
> fájl: SIGNAL_egy_szulo_egy_gyerek_szulo_signalt_kuld.c
> stackowerflow: https://stackoverflow.com/a/54374945/10438341
````C
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <sys/types.h>

void handler(int signumber){
  printf("Signal with number %i has arrived\n",signumber);
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
````
