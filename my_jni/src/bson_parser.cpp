#include "bson_parser.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <iostream>

#define T_K 1
#define T_V 0
#define T_L 2

#define assert(exp)

bson_parser::bson_parser(read_fun reader, int json_out)
{
	m_reader = reader;
	idx = 0;
	back = 0;
	memset(esc, 0 , sizeof(esc));
	this->json_out = json_out;
}


int bson_parser::start_parse(G_CB cb, void *ud)
{
	// printf("%s\n", __FUNCTION__);
	char type = 0;
	get_n_bytes(&type, 1);
	parse_type(type, T_L, cb, ud);

	return 0;
}

int bson_parser::get_n_bytes(char *buf, int len)
{
	// printf("%s %d\n", __FUNCTION__, len);
	if (m_reader == 0)
	{
		return -1;
	}

	if (len == 0) return 0;

	if (back)
	{
		buf[0] = back_c;
		len--;
		buf++;
		back = 0;
	}
	if (len) 
	{
		m_reader(buf, len);
		back_c = buf[len-1];
	}

	return 0;
}
int bson_parser::back_byte()
{
	// printf("%s\n", __FUNCTION__);
	back = 1;
	return 0;
}

int bson_parser::parse_map(G_CB cb, void *ud)
{
	// printf("%s\n", __FUNCTION__);
	char type;
	int f = 1;
	while (true)
	{
		get_n_bytes(&type, 1);
		
		if (type == 'e')
		{
			if (json_out)
			{
				esc[--idx] = '\0';
				printf("\n%s%s", esc, "}");
			}
			break;
		}

		if (f)
		{
			f = 0;
		}
		else
		{
			if (json_out) printf(",\n");
		}

		if (type >= '0' && type <= '9')
			parse_type(type, T_K, cb, ud);
		
		if (json_out) printf(": ");

		get_n_bytes(&type, 1);
		parse_type(type, T_V, cb, ud);

	}
	return 0;
}
int bson_parser::parse_list(G_CB cb, void *ud)
{
	// printf("%s\n", __FUNCTION__);
	char type;int f = 1;
	std::string key = g_key;
	// std::string n_key;
	// if (g_key[0] == '-') n_key = g_key + "-";
	// else n_key = "-";
	while (true)
	{
		get_n_bytes(&type, 1);
		if (type == 'e')
		{
			if (json_out)
			{
				esc[--idx] = '\0';
				printf("\n%s%s", esc, "]");
			}
			break;
		}

		if (f)
		{
			f = 0;
		}
		else
		{
			if (json_out) printf(",\n");
		}
		// g_key = n_key;
		parse_type(type, T_L, cb, ud);

	}
	return 0;
}
int bson_parser::parse_int(int pre)
{
	// printf("%s\n", __FUNCTION__);
	long long num = 0;
	char c;
	while (true)
	{
		get_n_bytes(&c, 1);
		if (c == 'e')
			break;
		num = num *10+c-'0';
	}
	if (json_out) printf("%s%lld", pre?esc:"", num);
	g_i_val = num;
	return 0;
}
int bson_parser::parse_string(int pre)
{
	// printf("%s\n", __FUNCTION__);
	char *p;
	int len = 0;
	char c;
	back_byte();

	while (true)
	{
		get_n_bytes(&c, 1);
		if (c == ':')
			break;
		len = len *10+c-'0';
	}
	p = (char*)calloc(len+1, 1);
	get_n_bytes(p, len);

	std::string str(p, p+len);

	if (json_out)
	{
		if (len < 500 && len !=16 && len !=20)
		{
			if (pre) std::cout << esc;
			std::cout << str;
		}
		else
		{
			if (!pre) printf("str(%d)", len);
			else printf("%s%s", pre?esc:"", p);
		}
	}
// if (pre)std::cerr << "\t" << pre<<"\t" <<str <<std::endl;
	if (pre == T_V && str.size()>100) str = "too long!";
	if (pre == T_K)
		g_key = str;
	else g_s_val = str;

	free(p);
	return 0;
}

int bson_parser::parse_type(char type, int pre, G_CB cb, void *ud)
{
	// printf("%s %d\n", __FUNCTION__, type);
	if (type == 'i')
	{
		parse_int(pre);
		if (pre == T_V && cb) cb(g_key, &g_i_val, T_INT_D, ud, NULL, NULL);
		if (pre == T_L && cb) cb(g_key, &g_i_val, T_INT_L, ud, NULL, NULL);
	}
	else if (type == 'd')
	{
		if (json_out)
		{
			if (pre == T_V) printf("%s\n", "{");
			else printf("%s%s\n", esc, "{");
			esc[idx++] = '\t';
		}

		G_CB n_cb = cb;
		void *n_ud = ud;
		if (pre == T_V && cb) cb(g_key, NULL, T_DIC_D, ud, (void**)&n_cb, &n_ud);
		if (pre == T_L && cb) cb(g_key, NULL, T_DIC_L, ud, (void**)&n_cb, &n_ud);

		parse_map(n_cb, n_ud);
		if (cb) cb(g_key, NULL, T_DIC_E, ud, (void**)&n_cb, &n_ud);
	}
	else if (type == 'l')
	{
		if (json_out)
		{
			if (pre == T_V) printf("%s\n", "[");
			else printf("%s%s\n", esc, "[");
			esc[idx++] = '\t';
		}

		G_CB n_cb = cb;
		void *n_ud = ud;
		if (pre == T_V && cb) cb(g_key, NULL, T_LST_D, ud, (void**)&n_cb, &n_ud);
		if (pre == T_L && cb) cb(g_key, NULL, T_LST_L, ud, (void**)&n_cb, &n_ud);

		parse_list(n_cb, n_ud);
		if (cb) cb(g_key, NULL, T_LST_E, ud, (void**)&n_cb, &n_ud);
	}
	else 
	{
		parse_string(pre);
		if (pre == T_V && cb) cb(g_key, &g_s_val, T_STR_D, ud, NULL, NULL);
		if (pre == T_L && cb) cb(g_key, &g_s_val, T_STR_L, ud, NULL, NULL);

	}
}