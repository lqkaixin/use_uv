#include <stdio.h>
#include <stdlib.h>

unsigned ip[3000000], nip;
unsigned ipseg[100000][3], nipseg;

int cmp(const void *a, const void *b)
{
	return *(unsigned*)a - *(unsigned*)b;
}
int n100000=10,nA=100;
#define A 100000
#define B(t) printf(#t ":%d\n", n##t) 
#define C(t) B(t)

int b_search(int idxa, int idxb, int idx)
{//printf("%d %d\n", idxa, idxb);

	int mid = (idxa + idxb)/2;
//printf("%d %d %d", ipseg[mid][0], ipseg[mid][1], ip[idx]);
	if (ipseg[mid][0] >ip[idx])
		return b_search(idxa, mid, idx);
	else if (ipseg[mid][1] <ip[idx])
		return b_search(mid+1, idxb, idx);
	else return mid;
}
int main()
{
  B(A);
  C(A);
printf ("ok %d\n",(int)(((unsigned short)2)-((unsigned short )3)));
	unsigned a,b;
	while(scanf("%d", &a) != EOF)
	{
		ip[nip++] = a;
	}
	
	while (scanf("%d %d", &a, &b) != EOF)
	{
		ipseg[nipseg][0] = a;
		ipseg[nipseg++][1] = b;
	}
	
	printf("\n");
	int i = 0;
	for (i=0;i<nipseg;i++)
	{
		//printf("aa %d %d %d\n", ipseg[i][0], ipseg[i][1], ipseg[i][2]);
	}
	qsort(ipseg, nipseg, 12, cmp);
	for (i=0;i<nipseg;i++)
	{
		//printf("bb %d %d %d\n", ipseg[i][0], ipseg[i][1], ipseg[i][2]);
	}
//	int i=0;
	//scanf("%d", &i);
	for (i=0;i<nip;i++)
	{
		ipseg[b_search(0, nipseg -1, i)][2] ++;
		//printf("%d\n", i);
	}
	for (i=0;i<nipseg;i++)
	{
		printf("%d %d %d\n", ipseg[i][0], ipseg[i][1], ipseg[i][2]);
	}
	return 0;
}
