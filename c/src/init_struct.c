#include <stdio.h>

struct TT 
{
  int a;
  int b;
};

int main()
{
  int p[10000]={0};
//  if (1){
    struct TT t;
  printf("%p, %d-%d\n",&t,t.a, t.b);
  //}
  struct TT s={0};
  printf("%p,%d-%d\n",&s,s.a, s.b);

  printf("-----%d\n",p[9999]);
  
  return 0;
}
