#include <stdio.h>
#include <time.h>

int main()
{
  time_t seconds;
  seconds = time(NULL);
  
  printf("Number of h since 1970 jan 1 is %ld \n",seconds/3600); 
  
  return 0;
}