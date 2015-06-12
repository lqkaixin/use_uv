#include <stdio.h>
#include <string.h>

int p[26][26];
int c[26];
int n;
char buf[30];

int main()
{
	while (scanf("%d", &n) && n)
	{
		memset(p,0,sizeof(p));
		for(int i=0;i<n;i++)
		{
			scanf("%s", buf);
			for(int j=2;buf[j];j++)
			{
				p[i][buf[j]-'A'] = 1;
				c[i]++;
			}
		}
		memset(c,-1,sizeof(c));
		int res = 26;
		for(int i=0;i<n;i++)
		{
			c[i]++;
			if (i==0 && c[0]==1)
			break;
			if (c[i]>i || c[i]>res)
			{
				c[i]=-1;
				i -= 2;
				continue;
			}
			for(int j=0;j<i;j++)
			{
				if (p[i][j] && c[j]==c[i])
				{
					i--;
					break;
				}
			}
			if (i==n-1)
			{
				int tmp = 0;
				for(int i=0;i<n;i++)
					if (c[i]>tmp) tmp = c[i];
				
				if (tmp < res)
					res = tmp;
				i--;
			}
		}
		
		if (res==0) printf("1 channel needed.\n");
		else printf("%d channels needed.\n",res+1);
		
	}
	return 0;
}
