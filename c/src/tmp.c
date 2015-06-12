#include "stdio.h"

int main()
{

char *p = 'c';

printf("%d\n", sizeof*p);

printf("%s\n", "sdtf\
	sdfdsf");
typeof(5) aa=4;
	char a[10] = {"bbbbbbbbb"};
	char (*b)[5];
	memset(&a,'a',sizeof(a));
	printf("%p-%p\n", b, &a);
	printf("%p-%p\n", a, &a);
	printf("%s\n", a);
  printf("%d\n",(int)((unsigned short)0-(unsigned short)100));
  return 0;
}
