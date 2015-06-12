#include <stdio.h>
#include <string.h>
#include <algorithm>

int q[20005];

int n;

struct Rect
{
	int left,lower,right,upper;
};

Rect rect[5000];

bool cmp1(const Rect &a, const Rect &b)
{
	return a.left < b.left;
}

bool cmp3(const Rect &a, const Rect &b)
{
	return a.lower < b.lower;
}

int main()
{
	int i = 0, j = 0;
	scanf("%d", &n);
	for(i=0;i<n;i++)
	{
		scanf("%d %d %d %d", &rect[i].left, &rect[i].lower, &rect[i].right, &rect[i].upper);
	}
	int res = 0;
	
	std::sort(rect, rect+n, cmp1);
	//memset(p, 0x7f, sizeof(p));//cur state
	memset(q, 0x80, sizeof(q));//other side state
	for(i=0;i<n;i++)
	{
		for (j=rect[i].lower + 10001;j<rect[i].upper + 10001;j++)
		{
			if (rect[i].left > q[j])
				res++;
			q[j] = std::max(rect[i].right,q[j]);
		}
	}
	
	std::sort(rect, rect+n, cmp3);
	//memset(p, 0x7f, sizeof(p));//cur state
	memset(q, 0x80, sizeof(q));//other side state
	for(i=0;i<n;i++)
	{
		for (j=rect[i].left + 10001;j<rect[i].right + 10001;j++)
		{
			if (rect[i].lower > q[j])
				res++;
			q[j] = std::max(rect[i].upper,q[j]);
		}
	}
	
	printf("%d\n", res+res);
	return 0;
}

















