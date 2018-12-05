#include <stdio.h>

int main(){
  printf("beolvas egy szoveget amit aztán kiir \n Adj meg valamit: ");
  char str[80];
  
  scanf("%4[^\n]", str);
  printf("Ezt olvastam: %s \n", str);

  return 0;
}