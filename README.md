# Info
egy jó leírás, elmélet + gyakorlati: https://github.com/angrave/SystemProgramming/wiki

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

# Csövek
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

