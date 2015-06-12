#include <stdio.h>
#include <algorithm>
#include <unistd.h>
struct PP
{
	int color;
	int num;
	int row,col;
};

int width;
PP p[1001];
int n;

PP q[10000];
int idx1;

PP s[10000];
int idx2;

PP w[10000];
int idx3;

int main()
{
	while (1){
	scanf("%d", &width);
	if (width == 0) {printf("0\n");break;}
	int a,b,r=0,c=0;
	n = 0;
	while(scanf("%d %d", &a, &b) == 2)
	{
		if (a==0 && b==0)
			break;
		p[n].row = r;
		p[n].col = c;
		p[n].color = a;
		p[n++].num = b;
		c += b;
		r += c/width;
		c %= width;
	}
	idx1 = 0;
	q[idx1].color = 0;
	q[idx1++].num = p[n-1].row*width+p[n-1].col+p[n-1].num;
	
	int di[8];
	di[0] = width + 1;
	di[1] = width;
	di[2] = width - 1;
	di[3] = 1;
	di[4] = -1;
	di[5] = -width + 1;
	di[6] = -width;
	di[7] = -width - 1;
	
	for(int j=0;j<8;j++)
	{int i=0,offi=0,cur=0,offc=0;
	//
	int pos1 = 0, pos2 = 0;
	int diff = di[j];
	idx2 = 0;
	while(pos2 < p[n-1].row*width+p[n-1].col+p[n-1].num)
	{
		//int pos1 = p[i].row*width+p[i].col+offi, pos2=p[cur].row*width+p[cur].col+offc;
		if (pos1 > pos2 - diff)
		{//printf("into1 %d %d %d %d\n", p[i].num, offi, p[cur].num, offc);
			if (pos1 - (pos2-diff) >= p[cur].num)
			{
				s[idx2].color = 0;
				s[idx2++].num = p[cur].num;
				pos2 += p[cur].num;
				cur++;
			}
			else
			{
				
				s[idx2].color = 0;
				s[idx2++].num = pos1 - (pos2-diff);
				offc = pos1 - (pos2-diff);
				pos2 += pos1 - (pos2-diff);
			}
		}
		else if (pos1 >= p[n-1].row*width+p[n-1].col+p[n-1].num)//end
		{//printf("into2 %d %d %d %d\n", p[i].num, offi, p[cur].num, offc);
			pos2 += p[cur].num-offc;
			s[idx2].color = 0;
			s[idx2++].num = p[cur].num-offc;
			cur++;
			offc = 0;
		}
		else if (pos1 < pos2 - diff)
		{//printf("into3 %d %d %d %d %d %d\n", p[i].num, offi, pos1, p[cur].num, offc, pos2);
		//printf("%d %d %d\n", diff, pos2-diff-pos1, p[i].num);
			if (pos2-diff-pos1 >= p[i].num)
			{
				pos1 += p[i].num;
				i++;
			}
			else 
			{
				offi += pos2-diff-pos1;
				pos1 += pos2-diff-pos1;
				
			}
		}
		else
		{//printf("into4 %d %d %d %d %d %d\n", p[i].num, offi, pos1, p[cur].num, offc, pos2);
			int offset = std::min(p[i].num-offi,p[cur].num-offc);
			int color = abs(p[cur].color-p[i].color);
			if (i!= cur)
			{
				if ((diff == width+1 || diff == 1 || diff == 1-width) && (pos2%width == 0) ||
				(diff == width-1 || diff == -1 || diff == -1-width) && (pos1%width == 0))
				{
					offset = 1;
					color = 0;
				}
				else
				{
				
				offset = std::min(offset,width-pos1%width);
				offset = std::min(offset,width-pos2%width);
				}
			}
			
			s[idx2].color = color;
			s[idx2++].num = offset;
			offi += offset;
			offc += offset;
			pos1 += offset;
			pos2 += offset;
			
			if (offi == p[i].num)
			{
				offi = 0;
				i++;
			}
			if (offc == p[cur].num)
			{
				offc = 0;
				cur++;
			}
		}
	}
	//printf("------%d %d\n",idx2, j);
	//for (i=0;i<idx2;i++)
	//	printf("%d %d\n", s[i].color,s[i].num);
		
	//merge
	i=0,offi=0,cur=0,offc=0,pos1 = 0;
	idx3 = 0;
	
	while (pos1<p[n-1].row*width+p[n-1].col+p[n-1].num)
	{
		int offset = std::min(q[i].num-offi,s[cur].num-offc);
		offi += offset;
			offc += offset;
			pos1 += offset;
			w[idx3].color = std::max(q[i].color,s[cur].color);
			w[idx3++].num = offset;
			
			if (offi == q[i].num)
			{
				offi = 0;
				i++;
			}
			if (offc == s[cur].num)
			{
				offc = 0;
				cur++;
			}
	}
	
	q[0].color = w[0].color;
	q[0].num = w[0].num;
	idx1 = 1;
	i=1;
	while (i<idx3)
	{
		if (w[i].color == q[idx1-1].color)
		{
			q[idx1-1].num += w[i].num;
			i++;
		}
		else 
		{
			q[idx1++] = w[i++];
		}
	}
	
	}
	
	printf("%d\n", width);
	for (int i=0;i<idx1;i++)
		printf("%d %d\n", q[i].color,q[i].num);
	printf("0 0\n");
	}
	return 0;
}



















