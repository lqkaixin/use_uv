
#include <stdio.h>



#define B A "bb"

#define A "aa"

#define C A "cc"

int tt_0()
{
printf("tt_0: 11\n");
}
int tt_1()
{
printf("tt_1: 11\n");
}
#define I 0

int i=0;

#define NN_(x) tt_##x();

#define NN(x) NN_(x) \
//if (x) NN(x-1);

int main()
{
	printf("B: %s\n", B);
	printf("C: %s\n", C);
	NN(1)
	return 0;
}
