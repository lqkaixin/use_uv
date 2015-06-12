#include "../include/MD5.h"
#include <memory>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <iostream>
using namespace std;

unsigned (* MD5::f[4])(unsigned b, unsigned c, unsigned d) = {&MD5::F, &MD5::G, &MD5::H, &MD5::I};

const unsigned MD5::PRE[] = {0x67452301, 0xefcdab89, 0x98badcfe, 0x10325476};

const unsigned MD5::data_idx[4][2] = {{0, 1}, {1, 5}, {5, 3}, {0, 7}};
const unsigned MD5::s[4][4] = {{7,12,17,22}, {5,9,14,20}, {4,11,16,23}, {6,10,15,21}};

const unsigned MD5::t[64] = {	0xd76aa478, 0xe8c7b756, 0x242070db, 0xc1bdceee,
								0xf57c0faf, 0x4787c62a, 0xa8304613, 0xfd469501,
								0x698098d8, 0x8b44f7af, 0xffff5bb1, 0x895cd7be,
								0x6b901122, 0xfd987193, 0xa679438e, 0x49b40821,

								0xf61e2562, 0xc040b340, 0x265e5a51, 0xe9b6c7aa,
								0xd62f105d, 0x02441453, 0xd8a1e681, 0xe7d3fbc8,
								0x21e1cde6, 0xc33707d6, 0xf4d50d87, 0x455a14ed,
								0xa9e3e905, 0xfcefa3f8, 0x676f02d9, 0x8d2a4c8a,

								0xfffa3942, 0x8771f681, 0x6d9d6122, 0xfde5380c,
								0xa4beea44, 0x4bdecfa9, 0xf6bb4b60, 0xbebfbc70,
								0x289b7ec6, 0xeaa127fa, 0xd4ef3085, 0x04881d05,
								0xd9d4d039, 0xe6db99e5, 0x1fa27cf8, 0xc4ac5665,

								0xf4292244, 0x432aff97, 0xab9423a7, 0xfc93a039,
								0x655b59c3, 0x8f0ccc92, 0xffeff47d, 0x85845dd1,
								0x6fa87e4f, 0xfe2ce6e0, 0xa3014314, 0x4e0811a1,
								0xf7537e82, 0xbd3af235, 0x2ad7d2bb, 0xeb86d391};


unsigned MD5::F(unsigned b, unsigned c, unsigned d)
{
	return (b&c)|((~b)&d);
}
unsigned MD5::G(unsigned b, unsigned c, unsigned d)
{
	return (b&d)|(c&(~d));
}
unsigned MD5::H(unsigned b, unsigned c, unsigned d)
{
	return b^c^d;
}
unsigned MD5::I(unsigned b, unsigned c, unsigned d)
{
	return c^(b|(~d));
}

MD5::MD5()
{
	reset();
}
MD5::~MD5() {}
void MD5::reset()
{
	memcpy(result, PRE, sizeof(PRE));
	data_len = 0;
}

void MD5::update(unsigned char *str, int len)
{
	int cur_len = data_len&0x3f;
	for (int idx = 0; idx < len;)
	{
		if (cur_len + len - idx >= 64)
		{
			memcpy(((char *)data) + cur_len, str + idx, 64 - cur_len);
			calc();
			idx += 64 - cur_len;
			cur_len = 0;
		}
		else
		{
			memcpy(((char *)data) + cur_len, str + idx, len - idx);
			idx = len;
		}
	}
	data_len += len;
}

void MD5::final()
{
	int cur_len = data_len&0x3f;
	memset(((char*)data)+cur_len, 0, 64-cur_len);
	memset(((char*)data)+cur_len, 0x80, 1);

	if (cur_len >= 56)
	{
		calc();
		memset(data, 0, sizeof(data));
	}

	data[14] = (data_len<<3)&0xffffffff;
	data[15] = (data_len>>29)&0xffffffff;
	calc();
	out_put_result();
}

void MD5::md5_string(char *str, int len)
{
	reset();
	update((unsigned char *)str, len);
	final();
}

bool MD5::md5_file(char * file_path)
{
	reset();
	unsigned char buff[1024];
	int len = 0, read_len = 0;
	FILE * f = fopen(file_path, "rb");
	if (f == NULL)
	{
		printf("open file error, errno: %d\n", errno);
		return false;
	}

	while ((read_len = fread(buff, 1, 1024, f)) > 0)
	{
		update(buff, read_len);
	}
	fclose(f);
	if (read_len < 0)
		return false;
	final();
	return true;
}

void MD5::calc()
{
	unsigned pre_data[4];
	memcpy(pre_data, result, 16);

	int j,k;
	for (int i=0;i<64;i++)
	{
		if ((i&15)==0)
			j = data_idx[i>>4][0], k = data_idx[i>>4][1];
		else j = (j+k)&15;
		unsigned tmp = pre_data[(-i)&3] + f[i>>4](pre_data[(-i+1)&3], pre_data[(-i+2)&3], pre_data[(-i+3)&3]) + data[j] + t[i];
		pre_data[(-i)&3] = pre_data[(-i+1)&3] + ((tmp << s[i>>4][i&3])|(tmp>>(32-s[i>>4][i&3])));
	}
	result[0] += pre_data[0];
	result[1] += pre_data[1];
	result[2] += pre_data[2];
	result[3] += pre_data[3];
}

void MD5::get_result(char out[])
{
	memcpy(out, result, sizeof(result));
	
}
void MD5::get_hex_result(char out[])
{
	for (int i=0;i<16;i++)
	{
		sprintf(out + (i<<1), "%02X", *((unsigned char *)result + i));
	}
}

void MD5::out_put_result()
{
	char r[33] = {0};
	get_hex_result(r);
	printf("%s\n", r);
}


int in_args(int argc, char *argv[])
{
	char arg[1024] = {0};

	int s = 0, e, idx = 0;
	while (idx < argc)
	{
		while (arg[s] == ' ') s++;
		if (arg[s] == '\0')
		{
			s = 0;
			gets(arg);
			continue;
		}
		e = s;
		if (arg[e] == '"' && ++s)
			while (arg[++e] != '"');
		else while (arg[e] != ' ' && arg[e] != '\0') e++;

		strncpy(argv[idx++], arg + s, e - s);
		if (arg[e] == '"') e ++;
		s = e;
	}
	return argc;
}


int main(int argc, char *argv[])
{
	char op[10] = {0}, arg[1024] = {"a"};
	
	
	MD5 md5;
	if (argc == 2)
	{
		// md5_file(argv[1]);
		md5.md5_file(argv[1]);
	}
	else if(argc == 3 && strcmp(argv[1], "-s")==0)
	{
		// md5_string(argv[2], strlen(argv[2]));
		md5.md5_string(argv[2], strlen(argv[2]));
	}
	else if(argc == 3 && strcmp(argv[1], "-b")==0)
	{
		char tmp[100] = {0};
		int len = strlen(argv[2]);
		for (int i=0;i<len/2;i++)
		{
			if (argv[2][i*2] >='0' && argv[2][i*2] <='9')
				tmp[i] += argv[2][i*2] -'0';
			else 
				tmp[i] += argv[2][i*2] -'A' + 10;

			tmp[i] *= 16;

			if (argv[2][i*2+1] >='0' && argv[2][i*2+1] <='9')
				tmp[i] += argv[2][i*2+1] -'0';
			else 
				tmp[i] += argv[2][i*2+1] -'A' + 10;
		}
		// md5_string(argv[2], strlen(argv[2]));
		md5.md5_string(tmp, len/2);
	}
	else if(argc == 1)
	{
		char * para[1] = {arg};
		in_args(1, para);
		// md5_file(arg);
		md5.md5_file(arg);
	}
	else
	{
		printf("invalid args!\n");
	}
	//getchar();
	//system("pause > null");
	return 0;
}