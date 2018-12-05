#include <stdio.h>

struct point {
  int x;
  int y;
};

void load(struct point *p); 

int main()
{
  printf("Ez a program a strukturákat teszteli le");
  struct point p;
  load(&p);
  printf("\n x: %d\n", p.x);
  printf("y: %d\n", p.y);
  
  return 0;
}

void load(struct point *p)
{
  p->x = 10;
  p->y = 20;
}
    