#include <stdio.h>

struct a
{
	double x;
char c;
};

int main()
{
	printf("%lu\n", sizeof(struct a));
	int s = 1;
	scanf("%d",&s);
	return 0;
}
