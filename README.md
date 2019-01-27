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
Kiolvashatunk **ismeretlen hosszúságút** is, pl így:
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

Nevesített csöveket
````C
int pid,fd;
int fid=mkfifo("fifo.ftc", S_IRUSR|S_IWUSR );
````
hozunk létre.
Itt egy **szülő ír** és egy **gyerek olvas**:
````C
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h> 

int main(int argc, char* argv[])
{
    int pid,fd;
    printf("Fifo start!\n");
    int fid=mkfifo("fifo.ftc", S_IRUSR|S_IWUSR ); // creating named pipe file
    // S_IWGRP, S_IROTH (other jog), file permission mode
    // the file name: fifo.ftc
    // the real fifo.ftc permission is: mode & ~umask 
    if (fid==-1)  //error handling
    {
      printf("Error number: %i",errno);
      exit(EXIT_FAILURE);
    }
    printf("Mkfifo vege, fork indul!\n\n");
    pid = fork();
    
  if(pid>0)   //parent 
  {
    printf("SZÜLŐ\tSzülő vagyok, irok a csobe!\n");
    printf("SZÜLŐ\tCsonyitas eredmenye: %d!\n",fid);
          fd=open("fifo.ftc",O_WRONLY);
          write(fd,"Hajra Fradi!\n",12);
          close(fd);
    printf("SZÜLŐ\tSzülő vagyok, beirtam, vegeztem!\n");
  }
  else // child
  {
    char s[1024]="Semmi";   
    printf("GYEREK\tCsonyitas eredmenye gyerekben: %d!\n",fid);
    fd=open("fifo.ftc",O_RDONLY);
    read(fd,s,sizeof(s));
    printf("GYEREK\tEzt olvastam a csobol: %s \n",s);
    close(fd);
    // remove fifo.ftc
    unlink("fifo.ftc");
  }   
  return 0; 
}
````
De **két névcső** használatával a **szülő írhat és olvashat is, ahogy a gyerek is**:
> fájl: NEVCSO_egy_szulo_egy_gyerek-gyerek_es_szulo_is_ir_ket_cso.c
````C
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h> 

int main(int argc, char* argv[])
{
    int pid,fd,fd2;
    printf("Fifo start!\n");
    int fid=mkfifo("fifo.ftc", S_IRUSR|S_IWUSR ); // creating named pipe file
    int fid2=mkfifo("fifo2.ftc", S_IRUSR|S_IWUSR ); // creating named pipe file
    // S_IWGRP, S_IROTH (other jog), file permission mode
    // the file name: fifo.ftc
    // the real fifo.ftc permission is: mode & ~umask 
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
    pid = fork();
    
  if(pid>0)   //parent 
  {
    //parent read
    char s[1024]="Semmi";   
    printf("SZULŐ\tCsonyitas eredmenye szuloben: %d!\n",fid);
    fd=open("fifo.ftc",O_RDONLY);
    read(fd,s,sizeof(s));
    printf("SZULŐ\tEzt olvastam a csobol: %s \n",s);
    close(fd);
    // remove fifo.ftc
    unlink("fifo.ftc");

    //parent write
    printf("SZULŐ\tSZULŐ vagyok, irok a 2. csobe!\n");
    printf("SZULŐ\tCsonyitas eredmenye: %d!\n",fid2);
          fd2=open("fifo2.ftc",O_WRONLY);
          write(fd2,"Hajra VASAS!\n",12);
          close(fd2);
    printf("SZULŐ\tSzülő vagyok, beirtam a 2.at, vegeztem!\n");


  }
  else // child
  {
    //child write
    printf("GYEREK\tGyerek vagyok, irok a csobe!\n");
    printf("GYEREK\tCsonyitas eredmenye: %d!\n",fid);
          fd=open("fifo.ftc",O_WRONLY);
          write(fd,"Hajra Fradi!\n",12);
          close(fd);
    printf("GYEREK\tGyerek vagyok, beirtam, az 1.-t!\n");

    //child read
    char s[1024]="Semmi";   
    printf("GYEREK\tCsonyitas2 eredmenye szuloben: %d!\n",fid2);
    fd2=open("fifo2.ftc",O_RDONLY);
    read(fd2,s,sizeof(s));
    printf("GYEREK\tEzt olvastam a csobol: %s \n",s);
    close(fd2);
    // remove fifo2.ftc
    unlink("fifo2.ftc");
  } 
    
    return 0; 
}
````

-------

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

> Signalok esetében a *handler*ben `printf`et használni nem egészéges! Bővebben stackoverflown: https://stackoverflow.com/a/16507805 vagy https://stackoverflow.com/a/9547988 vagy a dokumentációban: http://man7.org/linux/man-pages/man7/signal.7.html Vagy megoldhatjuk így:

>fájl: SIGNAL_egy_szulo_egy_gyerek_szulo_signalt_kuld-biztonsagos_handler.c
````C
#include <unistd.h>
#include <string.h>

#define WRITE(Str) (void)write(1,Str,strlen(Str))
void handler(int signumber)
{
    WRITE("Signal arrived\n");
}
````

**A `signal()`-t a `fork()` előtt kell meghívni, vagy mindenképp gondoskodni kell a szinkronizációról!**
> A szinkronizációról pl így lehet gondoskodni: http://www.code2learn.com/2011/01/signal-program-using-parent-child.html vagy így: https://stackoverflow.com/a/31102010

> **Figyelem!** Ezekben az esetekben a `return code`okat is figyelni kell, ugyanis azok a függvények és eljárások amik természetes esetben nem hasalhatnak el, ilyen esetekben elhasalhatnak, ha kigofynak pl memóriából, ilyen pl a `fork()` is!

Az alábbi példában egy **szülő küld** a **gyereknek** jelzést:
> fájl: SIGNAL_egy_szulo_egy_gyerek_szulo_signalt_kuld.c

> stackoverflow: https://stackoverflow.com/a/54374945/10438341
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
----
# Osztott memória
Az osztott memóriát így hozzuk létre:
````C
int pid;
key_t kulcs;
int oszt_mem_id;
char *s;

kulcs=ftok(argv[0],1); // a parancs nevevel es az 1 verzio szammal kulcs generalas
oszt_mem_id=shmget(kulcs,500,IPC_CREAT|S_IRUSR|S_IWUSR);  // osztott memoria letrehozasa, irasra olvasasra, 500 bajt mrettel
s = shmat(oszt_mem_id,NULL,0); // csatlakozunk az osztott memoriahoz, 
````
A gyereket azután hozzuk létre, hogy az osztott memóriához csatlakoztunk.

Teljes programkód, ahol a **szülő ír** az **osztott memóriába** és a **gyerek olvas**:
> fájl: osztott_memoria_egy_gyerek_egy_szulo_szulo_ir.c
````C
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <wait.h>


int main (int argc,char* argv[]) {

    int pid;
    key_t kulcs;
    int oszt_mem_id;
    char *s;
	// a parancs nevevel es az 1 verzio szammal kulcs generalas
    kulcs=ftok(argv[0],1);
    // osztott memoria letrehozasa, irasra olvasasra, 500 bajt mrettel
    oszt_mem_id=shmget(kulcs,500,IPC_CREAT|S_IRUSR|S_IWUSR);
    // csatlakozunk az osztott memoriahoz, 
    // a 2. parameter akkor kell, ha sajat cimhez akarjuk illeszteni
    // a 3. paraméter lehet SHM_RDONLY, ekkor csak read van   
 s = shmat(oszt_mem_id,NULL,0);
    //
    pid = fork();
    if(pid>0){    
    
       char buffer[] = "Hajra Fradi! \n";                   
       // beirunk a memoriaba 
       strcpy(s,buffer);
       // elengedjuk az osztott memoriat
       shmdt(s);	   
//	s[0]='B';  //ez segmentation fault hibat ad
       wait(NULL); 
	// IPC_RMID- torolni akarjuk a memoriat, ekkor nem kell 3. parameter
	// IPC_STAT- osztott memoria adatlekerdezes a 3. parameterbe,
	//  ami shmid_ds struct tipusu mutato
       shmctl(oszt_mem_id,IPC_RMID,NULL);
    } 
    else 
	if ( pid == 0 ) 
	{
	sleep(1);	              
        printf("A gyerek ezt olvasta az osztott memoriabol: %s",s);
	// gyerek is elengedi
        shmdt(s);
	}

   return 0;
}
````
A **gyerek** is **ír**hat, és a **szülő olvashatja ki**. Ilyenkoir figyelni kell, hogy a gyereket meg kell várni, `wait(NULL);`!
> fájl: OSZTMEM_egy_gyerek_egy_szulo-gyerek_ir.c
````C
  pid = fork();
  if(pid>0){   
    
     wait(NULL); 
     printf("A szülő ezt olvasta az osztott memoriabol: %s",s);
     shmdt(s);   // szülő elengedi
  } 
  else 
	if ( pid == 0 ) 
	{
	     sleep(1);	      
       char buffer[] = "Hajra Fradi! \n";                   
       // beirunk a memoriaba 
       strcpy(s,buffer);
       // gyerekkel elengedjuk az osztott memoriat
       shmdt(s);  
       shmctl(oszt_mem_id,IPC_RMID,NULL);
	}
   return 0;
}
````

# Üzenetsor
