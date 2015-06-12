#include "stdio.h"
#include "stdlib.h"
#include "string.h"

#include "utility.h"

#define DEBUG(...) printf(__VA_ARGS__)

unsigned char S[256];
int i, j;

void initSBox(char *key, int len)
{
	DEBUG("%s, %d\n", key, len);
	for (i = 0; i <= 255; i++)
		S[i] = i;
	for (i = 0; i <= 255; i++)
	{
		j = (j + S[i] + (unsigned char)key[i%len])%256;
		int tmp = S[i];
		S[i] = S[j];
		S[j] = tmp;
	}
	i = j = 0;
}

void updatestate()
{
	i = (++i)%256;
	j = (j + S[i]) % 256;
	DEBUG("[updatestate] %d %d\n", i, j);
	int tmp = S[i];
	S[i] = S[j];
	S[j] = tmp;
	DEBUG("[updatestate] %d\n", i);
}

void encode(char *src, char *des, int len)
{
	int k = 0;
	for (k=0;k<len;k++)
	{
		updatestate();
		des[k] = src[k]^S[(S[i]+S[j])%256];
		DEBUG("[decode] src: %d, des: %d\n", src[k], des[k]);
	}
}




void test_rc4()
{
	char p[260], f[5];
	int len = 0;
	char src[1024],des[1024];
	
	printf("Please tell key format(b base64, h hex, other string):");
	scanf("%s", f);
	
	printf("Please input the key: ");
	scanf("%s", p);
	
	len = strlen(p);

	if (f[0]=='b')
		len = base642str(p, len);
	else if (f[0]=='h')
		len = hex2str(p, len);

	initSBox(p, len);

	printf("Please tell input text format(b base64, h hex, other string):");
	scanf("%s", f);
	
	printf("Please input src data!\n");
	scanf("%s", src);
	
	len = strlen(src);

	DEBUG("input ok %s, %d!\n", src, len);

	if (f[0]=='b')
		len = base642str(src, len);
	if (f[0]=='h')
		len = hex2str(src, len);

	encode(src, des, len);

	printf("Please tell output text format(b base64,h hex, other string):");
	scanf("%s", f);

	if (f[0]=='b')
	{
		len = str2base64(des, len);
		printf("base64 result:");

	}
	if (f[0]=='h')
	{
		len = str2hex(des, len);
		printf("hex result:");
	}
	
	printf("%s\n", des);
}

int main()
{
	unsigned short a=1,b=5;
	printf("%d, %d", (int)(a-b),(int)sizeof(a-b));

	char f[1024] = {"U2FsdGVkX18LOCVG5niv8gUdEg=="};
	int len = base642str(f, strlen(f));
	printf("::%s\n\n", f);
	len = str2hex(f, len);
	printf("::%s\n\n", f);
	
	test_rc4();

	return 0;
}
