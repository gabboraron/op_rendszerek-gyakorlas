//test
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <string.h>
#include <time.h>

#define MAX_STRING_LEN 80
#define PHONE_NUMBER 15

typedef struct order Order;
struct order {
    time_t	 systime;
    char	 name[MAX_STRING_LEN];
    char	 email[MAX_STRING_LEN];
    int		 phonenumber;
    int		 size;    
};


//functions
void readName(struct order *current);
void checkValues(struct order *current);
void readEmail(struct order *current);
void readPhone(struct order *current);
void readSize(struct order *current);
void saveToFiles(struct order *current);
void getNewOrder(struct order *current);
void dbList(struct order *current);
void deleteOrder(struct order *current);
void modificator(struct order *current);

//read name
void readName(struct order *current){
    printf("Az ügyfél neve: ");
    scanf("%80s[^\n]", &current->name);
   // scanf("%s",current->name);
}

//read email
void readEmail(struct order *current){
    printf("Az ügyfél e-mail címe: ");
    char tmp[80];
    scanf("%s[^\n]", &current->email);
}

//read phone number
void readPhone(struct order *current){
    printf("Az ügyfél telefonszáma: ");
    scanf("%15i[^\n]", &current->phonenumber);
}

//read size of order
void readSize(struct order *current){
    printf("A rendelés mérete: ");
    scanf("%i", &current->size);
}

//check the submited values
void checkValues(struct order *current){
    printf("\n *** \n");
    printf("Név: %s \n", current->name);
    printf("e-mail: %s \n", current->email);
    printf("tel: %d \n", current->phonenumber);
    printf("méret: %d \n", current->size);
    printf("idő: %ld \n", current->systime);
    printf(" *** \n \n");
}

//save into file
void saveToFiles(struct order *current){    
    FILE * file=fopen("db.txt", "a");
    if(file != NULL){
        fwrite(current, sizeof(struct order), 1, file);
        fclose(file);
    }
}

//read new values
void getNewOrder(struct order *current){
//   struct order tmp = &current;
   //read values  
   readName(current);
   readEmail(current); 
   readPhone(current);
   readSize(current);
   current->systime = time(NULL);
   // ** //
   checkValues(current);

   
   printf("Ezek megfelelőek? (i/n) ");
   char answer;
   char yes = 'i';
   scanf(" %c", &answer);
   
   if(answer == yes){ 
      printf("Mentés...\n");
      saveToFiles(current); //save to file
      printf("Mentve\n \n");
   } else {
      printf("Add meg újra! \n");
      getNewOrder(current);	
   }
}

//list the db
void dbList(struct order *current){
    int option;
    printf("|- Kilistázás: \n    1 - teljes lista\n    2 - szűrés ügyfélre\n    3 - szűrés teljesítmény ígényre\n    0 - kilép");
    scanf(" %i", &option);
    if(option == 0){
        exit(1);
    }
    if(option == 1){    
        struct order *obj=malloc(sizeof(struct order));
        FILE * file = fopen("db.txt","rb");
        fseek(file, 0, SEEK_SET);
        //while(!feof(file)){
        while(fread(obj, sizeof(struct order), 1, file)){
//        char buffer[300];
//        while(fread(buffer, sizeof buffer, 1, stream == 1){
//printf("LOG: op1\n");
//            fread(obj, sizeof(struct order), 1, file);
            printf("%s/%s/%d/%d\n", obj->name, obj->email, obj->phonenumber, obj->size);
        }
        if(feof(file)){printf("\n--END--\n");}
        else{printf("Some error...");} 
        
        /*char buffer[300];
        FILE * file = fopen("db.txt", "rb");
        while(fscanf(file,"%300s",bufer) == 1){
            printf("%s",buffer);
        }*/
    }
    if(option == 2){
       printf("ügyfél neve: ");
       char name[80];
       scanf(" %s",name);
       struct order *obj=malloc(sizeof(struct order));
       FILE * file = fopen("db.txt", "rb");
       fseek(file, 0, SEEK_SET);
       while(fread(obj,sizeof(struct order), 1, file)){
           if(strcmp(obj->name, name) == 0){
               printf("%s/%s/%d/%d\n", obj->name, obj->email, obj->phonenumber, obj->size);
           }
       }
       if(feof(file)){printf("\n--END--\n");}else{printf("ERROR");}
       fclose(file);
    }
    if(option == 3){
       printf("teljesítmény ígény: ");
       char size[80];
       scanf(" %s",size);
       struct order *obj=malloc(sizeof(struct order));
       FILE * file = fopen("db.txt", "rb");
       fseek(file, 0, SEEK_SET);
       while(fread(obj,sizeof(struct order), 1, file)){
            char tmp[sizeof(obj->size)];
            sprintf(tmp, "%d", obj->size);
            if(strcmp(tmp, size) == 0){
                printf("%s/%s/%d/%d\n", obj->name, obj->email, obj->phonenumber, obj->size);
            }
       }
       if(feof(file)){printf("\n--END--\n");}else{printf("ERROR");}
       fclose(file);
    }
}

//get the size of the db
int getDbSize(){
    int size = 0;
    struct order*obj=malloc(sizeof(struct order));
    FILE * file = fopen("db.txt", "rb");
    fseek(file, 0, SEEK_SET);
    while(fread(obj,sizeof(struct order), 1, file)){
         ++size;
    }   
    fclose(file);
    return size;
}

//modification of db
void modificator(struct order *current){
    printf("Módosítani kívánt ügyfél e-mail címe: ");
    char mail[80];
    scanf(" %s",mail);
    //modification
    int size=getDbSize();
    struct order arr_order[size];
    struct order *obj=malloc(sizeof(struct order));
    FILE * file = fopen("db.txt", "rb");
    fseek(file, 0, SEEK_SET);
    int idx = 0;
    while(fread(obj, sizeof(struct order), 1, file)){
         if(strcmp(obj->email, mail) == 0){
 //printf("\n *** \n mail match: %s AND %s\n *** \n", obj->email, mail);           
            //change
         }else{
            //copy the rest
            strcpy(arr_order[idx].name, obj->name);
            //arr_order[idx].name = obj->name;
            strcpy(arr_order[idx].email, obj->email);
            //arr_order[idx].email = obj->email;
            arr_order[idx].phonenumber = obj->phonenumber;
            arr_order[idx].size = obj->size;
            arr_order[idx].systime = obj->systime; 
         ++idx;
         }
    }
    
 //printf("\n *** \n the size of the db: %i \n *** \n", idx);
    size=idx;   
    fclose(file);
    fseek(file, 0, SEEK_SET);
    FILE * newfile=fopen("db.txt", "w+");
    idx=0;
    if(newfile != NULL){
    	for(idx=0; idx<size; ++idx){
    	        //printf("id: %i", idx);
    	        //checkValues(&arr_order[idx]);
        	fwrite(&arr_order[idx], sizeof(struct order), 1, newfile);
	}
    }
    fclose(newfile);
    getNewOrder(current);
    printf("Módosítások mentve\n");
}

//delte an order
void deleteOrder(struct order *current){
    printf("Törölni kívánt ügyfél e-mail címe: ");
    char mail[80];
    scanf(" %s",mail);
    //deleteOrder
    int size=getDbSize();
    struct order arr_order[size];
    struct order *obj=malloc(sizeof(struct order));
    FILE * file = fopen("db.txt", "rb");
    fseek(file, 0, SEEK_SET);
    int idx = 0;
    while(fread(obj, sizeof(struct order), 1, file)){
         if(strcmp(obj->email, mail) == 0){
 //printf("\n *** \n mail match: %s AND %s\n *** \n", obj->email, mail);           
            //delete
            printf("Törölve. \n");
         }else{
            //copy the rest
            strcpy(arr_order[idx].name, obj->name);
            //arr_order[idx].name = obj->name;
            strcpy(arr_order[idx].email, obj->email);
            //arr_order[idx].email = obj->email;
            arr_order[idx].phonenumber = obj->phonenumber;
            arr_order[idx].size = obj->size;
            arr_order[idx].systime = obj->systime; 
            ++idx;
         }
    }
    
 //printf("\n *** \n the size of the db: %i \n *** \n", idx);
    size=idx;   
    fclose(file);
    
    FILE * newfile=fopen("db.txt", "w+");
    fseek(file,0, SEEK_SET);
    idx=0;
    if(newfile != NULL){
    	for(idx=0; idx<size; ++idx){
        	fwrite(&arr_order[idx], sizeof(struct order), 1, newfile);
        	//printf("id: %i", idx);
        	//checkValues(&arr_order[idx]);
	    }
    }
    fclose(newfile);
    printf("Módosítások mentve\n");
}

//***
//*** BEAD2
//msg struct
struct uzenet { 
     long mtype;//ez egy szabadon hasznalhato ertek, pl uzenetek osztalyozasara
     char mtext [ 1024 ]; 
}; 

// sendig a message
int kuld( int uzenetsor, int szuloVGyerek ) 
{ 
     const struct uzenet uz= { 5, "Hajra Fradi!" }; 
     int status; 
     
     status = msgsnd( uzenetsor, &uz, strlen ( uz.mtext ) + 1 , 0 ); //üzenet küldése //msgsnd( uzenetsor neve, &amibe, memóriaszelet mérete /*fix méret kell!*/, IPC_NOWAIT constans /*nincs várakozás*/ );
	     // a 3. param ilyen is lehet: sizeof(uz.mtext)
     	// a 4. parameter gyakran IPC_NOWAIT, ez a 0-val azonos
     if ( status < 0 ) 
          perror("msgsnd"); 
     return 0; 
}

//optimize orders
/*Két azonos teljesítményű rendszer telepítése egy napi munkát jelent egy szerelő csapatnak (gyerek). Ugyanakkor ha rendelés felvételi ideje óta legalább egy hét eltelt, akkor ezt a rendelést akár napi egy feladatként, soron kívül is kiadja**/
struct order currentJob[2]; // current jobs
int nrOfJobs = 1;           // how many are of them
void optimize(struct order *current){ 
	struct order *tmp_oldest=malloc(sizeof(struct order));
	struct order *tmp_fst = malloc(sizeof(struct order));
	struct order *tmp_snd = malloc(sizeof(struct order));


	int idx = 0;
	struct order *obj=malloc(sizeof(struct order));
    FILE * file = fopen("db.txt","rb");
    fseek(file, 0, SEEK_SET);
  //get the oldest value  
	printf("name \t email \t phonenumber \t size \t systime\n");
  while(fread(obj, sizeof(struct order), 1, file)){  
	  printf("%s \t %s \t %d \t %d \t %ld\n", obj->name, obj->email, obj->phonenumber, obj->size, obj->systime);
		if(idx == 0)
		{//fst
			strcpy(tmp_oldest->name, obj->name);
			strcpy(tmp_oldest->email, obj->email);
			tmp_oldest->phonenumber = obj->phonenumber; 
			tmp_oldest->size = obj->size;
			tmp_oldest->systime = obj->systime;
			++idx;
		 //get the current value

		}else
		{//other
			if (tmp_oldest->systime > obj->systime)
			{//if current is older then the oldest then set it as oldest
				strcpy(tmp_oldest->name, obj->name);
        strcpy(tmp_oldest->email, obj->email);
				tmp_oldest->phonenumber = obj->phonenumber; 
				tmp_oldest->size = obj->size;
				tmp_oldest->systime = obj->systime;
			}
      ++idx;
		}
  }
  printf("Legregebbi:\n");
  checkValues(tmp_oldest);
  printf("nr of elements: %d  \n",idx);

  /***/
  //get two of same size
  int size=getDbSize();
  struct order data[size];  
  idx = 0;
  fseek(file, 0, SEEK_SET);
  while(fread(obj, sizeof(struct order), 1, file)){  //copy into an array
    //printf("copying: %s \t %s \t %d \t %d \t %ld\n", obj->name, obj->email, obj->phonenumber, obj->size, obj->systime);
    strcpy(data[idx].name, obj->name);
    strcpy(data[idx].email, obj->email);
    data[idx].phonenumber = obj->phonenumber;
    data[idx].size = obj->size;
    data[idx].systime = obj->systime;
    ++idx;
  }

  if(feof(file)){printf("\n--END--\n"); fclose(file);}
    else{printf("Error while search in file");} 

  //printf("data  array: \n");  
  int gotIt = 0;
  if(idx>0)
  {
    strcpy(tmp_fst->name, data[0].name);
    strcpy(tmp_fst->email, data[0].email);
    tmp_fst->phonenumber = data[0].phonenumber;
    tmp_fst->size = data[0].size;
    tmp_fst->systime = data[0].systime;
    idx = 1;
    while((gotIt == 0) || (idx<size)){//search in array for same size values
    //for( idx = 1; idx < size; idx = idx + 1 ){
      //checkValues(&data[idx]);
      int jdx;
      for( jdx = idx; jdx < size; jdx = jdx + 1 ){//get the second with this size
        if(tmp_fst->size == data[jdx].size){
          strcpy(tmp_snd->name, data[jdx].name);
          strcpy(tmp_snd->email, data[jdx].email);
          tmp_snd->phonenumber = data[jdx].phonenumber;
          tmp_snd->size = data[jdx].size;
          tmp_snd->systime = data[jdx].systime;
          gotIt = 1;
        }    
      }
      ++idx;
    }
  }  
    
  printf("ket megegyezo meretu rendeles:\n");
  checkValues(tmp_fst);
  checkValues(tmp_snd);
  /***/
  if(tmp_oldest->systime/3600 >= 168){//if the oldest is one week old then this is the current job
    strcpy(currentJob[0].name, tmp_oldest->name);
    strcpy(currentJob[0].email, tmp_oldest->email);
    currentJob[0].phonenumber = tmp_oldest->phonenumber;
    currentJob[0].size = tmp_oldest->size;
    currentJob[0].systime = tmp_oldest->systime;
    printf("The oldest set as current job.\n");
    checkValues(&currentJob[0]);
  } else { //else send the other two with same size
    if (gotIt == 0 )
    {//whe haven't two order with the same size
      strcpy(currentJob[0].name, tmp_fst->name);
      strcpy(currentJob[0].email, tmp_fst->email);
      currentJob[0].phonenumber = tmp_fst->phonenumber;
      currentJob[0].size = tmp_fst->size;
      currentJob[0].systime = tmp_fst->systime;
      printf("In the db isn't two with same size so, only one job is set as current job.\n");
      checkValues(&currentJob[0]);
    } else {// we have two order with same size
      /*Order *new_jobs = realloc(currentJob, (sizeof(currentJob))+1 * sizeof *new_jobs);
      if (new_jobs == NULL)
      {
          printf("Error while realloc job list.\n");
      }
      currentJob = new_jobs;*/
      //struct order currentJob[1] = (struct order) realloc (NULL, 2*sizeof(struct order));
      /*int old_size = sizeof(currentJob);
      struct order *new_data = malloc(old_size + 1 * sizeof *new_data);
      if (new_data == NULL)
      {
          printf("Error while realloc.\n");
      }
      memcpy(new_data, currentJob, old_size * sizeof *currentJob);
      free(currentJob);
      currentJob = new_data;
      ++old_size;*/

      ++nrOfJobs; //set nr of jobs to 2

      strcpy(currentJob[0].name, tmp_fst->name);
      strcpy(currentJob[0].email, tmp_fst->email);
      currentJob[0].phonenumber = tmp_fst->phonenumber;
      currentJob[0].size = tmp_fst->size;
      currentJob[0].systime = tmp_fst->systime;

      strcpy(currentJob[1].name, tmp_snd->name);
      strcpy(currentJob[1].email, tmp_snd->email);
      currentJob[1].phonenumber = tmp_snd->phonenumber;
      currentJob[1].size = tmp_snd->size;
      currentJob[1].systime = tmp_snd->systime;
      printf("Two with same size set as current job.\n");
      checkValues(&currentJob[0]);
      checkValues(&currentJob[1]);
    }
  }
}

// receiving a message. 
/*int fogad( int uzenetsor, int szuloVGyerek) 
{ 
     struct uzenet uz; 
     int status; 
     // az utolso parameter(0) az uzenet azonositoszama
	// ha az 0, akkor a sor elso uzenetet vesszuk ki
	// ha >0 (5), akkor az 5-os uzenetekbol a kovetkezot
	// vesszuk ki a sorbol 
     status = msgrcv(uzenetsor, &uz, 1024, 5, 0 ); //msgrcv(mit, &hova, memóriaszelet, melyik típusú üzenetet az üzenetsorból //3-mtype mező
     , 0. elem );
     
     if ( status < 0 ) 
          perror("msgsnd"); 
     else
          printf( "A kapott uzenet kodja: %ld, szovege:  %s\n", uz.mtype, uz.mtext ); 
     return 0; 
} */
//***
int main(k)
{
    printf("Fenyes Nap Kft \n Jellemzően egy évre vonatkoztatva 1000KW/h energiát 1KW teljesítményt nyujtó 4 darab napelem panel \n \n");
    struct order current; 	//struct init
//    current = malloc(sizof(*current));

   int option = 1;
   while(option != 0){
        printf("~ FENYES NAP ~ \n 0 kilépés \n 1 új felvétele \n 2 kilistázás \n 3 ügyfél módosítás \n 4 ügyfél törlése \n \n 5 optimize \nválasztás: ");
   	scanf(" %i", &option);	
   	if(option == 1){getNewOrder(&current);}
        if(option == 2){dbList(&current);}
        if(option == 3){modificator(&current);}
        if(option == 4){deleteOrder(&current);}
        if(option == 5){optimize(&current);}

        if(option == 0){printf("Kilépés\n");}
        else{printf("\n Hibás parancs\n");}
   }    
    return 0;
}
