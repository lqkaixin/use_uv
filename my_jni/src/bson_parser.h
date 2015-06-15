#include <string>

// 0 NULL, 1 int, 2 std::string

typedef int (*G_CB)(std::string &key, void*value, int v_type, void *ud, void **next_cb, void **next_ud);
typedef int (*read_fun)(char *buf, int len);

#define T_STR_D 0
#define T_INT_D -1
#define T_DIC_D -2
#define T_LST_D -4

#define T_INT_L -6
#define T_STR_L -7
#define T_DIC_L -8
#define T_LST_L -9

#define T_DIC_E -3
#define T_LST_E -5

class bson_parser
{
	std::string g_key;
	std::string g_s_val;
	long long g_i_val;

	read_fun m_reader;
	int back;
	char back_c;

	char esc[100];
	int idx;

	int json_out;

	int get_n_bytes(char *buf, int len);
	int back_byte();

	int parse_map(G_CB cb, void *ud);
	int parse_list(G_CB cb, void *ud);
	int parse_int(int pre);
	int parse_string(int pre);
	int parse_type(char type, int pre, G_CB cb, void *ud);

public:
	bson_parser(read_fun reader, int json_out);
	int start_parse(G_CB cb, void *ud);
};