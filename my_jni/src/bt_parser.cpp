#include <stdio.h>
#include <string.h>
#include <string>
#include <vector>
#include <string.h>
#include <iostream>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

#include "bson_parser.h"

int cb_out  = 0;
int json_out = 0;

typedef struct bt_file_t
{
	long long length;//:文件长度，单位字节(整数)
	std::string path;

	std::string md5sum;//(可选)：长32个字符的文件的MD5校验和，BT不使用这个值，只是为了兼容一些程序所保留!(字符串)
	int piece_length;//:每个块的大小，单位字节(整数)
	std::string pieces;//:每个块的20个字节的SHA1 Hash的值(二进制格式)
/*
name:文件名(字符串)

path:文件的路径和名字，是一个列表结构，如/test/test.txt 列表为l4:test8test.txte
name:最上层的目录名字(字符串)*/
}bt_file;

typedef struct bt_info_t
{
	std::string announce;//tracker服务器的URL(字符串)
	std::vector<std::string> announce_list;//(可选):备用tracker服务器列表(列表)
	unsigned int creation_date;//(可选):种子创建的时间，Unix标准时间格式，从1970 1月1日 00:00:00到创建时间的秒数(整数)
	std::string comment;//(可选):备注(字符串)
	std::string created_by;//(可选):创建人或创建程序的信息(字符串);
	std::string encoding;

	std::vector<bt_file *> file_infos;
	long long length;

	std::string name;
	int piece_length;
	int pieces;
	std::string publisher;

}bt_info;


int notify_bt_info(std::string &key, void*value, int v_t, void *ud, void **next_cb, void **next_ud);
int notify_bt_announce_list(std::string &key, void*value, int v_t, void *ud, void **next_cb, void **next_ud);
int notify_bt_files(std::string &key, void*value, int v_t, void *ud, void **next_cb, void **next_ud);


int notify_bt_root(std::string &key, void*value, int v_t, void *ud, void **next_cb, void **next_ud)
{
	if (cb_out){
		std::cerr << "root ud: "  << ", [" << key<<": ";
		if(v_t == T_INT) std::cerr<<*(int*)value;
		else if (v_t == T_STR)
		{
	#define AA (*(std::string*)value)
			if (AA.length()>500 || AA.length()==20 || AA.length()==16)
				std::cerr<<AA.length();
			else std::cerr<<AA;
		}
		else if (v_t == T_DIC)std::cerr<<"-dic";
		else if (v_t == T_LST)std::cerr<<"-list";
		std::cerr<<"]"<<std::endl;
	}
	// fprintf("ud: %p, %s: \n", ud, key.c_str());

	bt_info *info = (bt_info*)ud;

	if (key == "announce") info->announce = AA;
	else if (key == "created by") info->created_by = AA;
	else if (key == "creation date") info->creation_date = *(int*)value;
	else if (key == "encoding") info->created_by = AA;

	else if (key == "info")
	{
		*next_cb = (void*)notify_bt_info;
		*next_ud = ud;
	}

	else if (key == "announce-list")
	{
		*next_cb = (void*)notify_bt_announce_list;
		*next_ud = ud;
	}
	else
	{
		if (!json_out && !cb_out)printf("[%s] %s\n", __FUNCTION__, key.c_str());
	}

	return 0;
}

int notify_bt_announce_list(std::string &key, void*value, int v_t, void *ud, void **next_cb, void **next_ud)
{
	if (cb_out){
		std::cerr << " alist ud: "  << ", [" << key<<": ";
		if(v_t == T_INT) std::cerr<<*(int*)value;
		else if (v_t == T_STR)
		{
			if (AA.length()>500 || AA.length()==20 || AA.length()==16)
				std::cerr<<AA.length();
			else std::cerr<<AA;
		}
		else if (v_t == T_DIC)std::cerr<<"-dic";
		else if (v_t == T_LST)std::cerr<<"-list";
		std::cerr<<"]"<<std::endl;
	}

	if (v_t == T_STR)
	{
		bt_info *info = (bt_info*)ud;
		info->announce_list.push_back(AA);
	}
	return 0;
}

int notify_bt_info(std::string &key, void*value, int v_t, void *ud, void **next_cb, void **next_ud)
{
	if (cb_out){
		std::cerr << " info ud: "  << ", [" << key<<": ";
		if(v_t == T_INT) std::cerr<<*(int*)value;
		else if (v_t == T_STR)
		{
			if (AA.length()>500 || AA.length()==20 || AA.length()==16)
				std::cerr<<AA.length();
			else std::cerr<<AA;
		}
		else if (v_t == T_DIC)std::cerr<<"-dic";
		else if (v_t == T_LST)std::cerr<<"-list";
		std::cerr<<"]"<<std::endl;
	}

	bt_info *info = (bt_info*)ud;

	if (key == "name") info->name = AA;
	else if (key == "publisher") info->publisher = AA;
	else if (key == "piece length") info->piece_length = *(int*)value;
	else if (key == "pieces") info->pieces = *(int*)value;
	else if (key == "length") info->length = *(int*)value;

	else if (key == "files")
	{
		bt_file *p = new bt_file;
		info->file_infos.push_back(p);
		*next_cb = (void*)notify_bt_files;
		*next_ud = p;
	}
	else 
	{
		if (!json_out && !cb_out)printf("[%s] %s\n", __FUNCTION__, key.c_str());
	}
	// printf("ud: %p, %s: \n", ud, key.c_str());
	return 0;
}

int notify_bt_files(std::string &key, void*value, int v_t, void *ud, void **next_cb, void **next_ud)
{
	if (cb_out){
		std::cerr << "  files ud: "  << ", [" << key<<": ";
		if(v_t == T_INT) std::cerr<<*(int*)value;
		else if (v_t == T_STR)
		{
			if (AA.length()>500 || AA.length()==20 || AA.length()==16)
				std::cerr<<AA.length();
			else std::cerr<<AA;
		}
		else if (v_t == T_DIC)std::cerr<<"-dic";
		else if (v_t == T_LST)std::cerr<<"-list";
		std::cerr<<"]"<<std::endl;
	}

	bt_file *info = (bt_file*)ud;

	if (key == "length") info->length = *(int*)value;
	else if (key == "path")
	{
		if (v_t == T_STR) info->path = AA;
	}
	else 
	{
		if (!json_out && !cb_out)printf("[%s] %s\n", __FUNCTION__, key.c_str());
	}

	// printf("ud: %p, %s: \n", ud, key.c_str());
	return 0;
}

int fid;

int get_n_bytes(char *buf, int len)
{
	if (fid == 0)
	{
		fid = open("/media/lq-kaixin/OS/tmp/ss/波多野結衣加勒比2011和服中出高清960×540/0FF035D6064716A80441B651CF009F437A12E23D.torrent", O_RDONLY,0);
		// fid = open("/home/lq-kaixin/work/libuv_study/test.seed", O_RDONLY,0);
	}

	if (len) read(fid, buf, len);
	return 0;
}

int get_n_bytes1(char *buf, int len)
{
	static int pos = 0;
	static int total = 0;
	static char p[] = "d8:announce35:http://www.manfen.net:7802/announce13:creation datei1076675108e4:infod6:lengthi17799e4:name62:MICROSOFT.WINDOWS.2000.AND.NT4.SOURCE.CODE-SCENELEADER.torrent12:piece lengthi32768e6:pieces20:                    ee";

	if (total == 0) total = sizeof(p) -1;


	memcpy(buf, p+pos, len);
	pos += len;

	char buff[1024] = {"echo "};
	memcpy(buff+5, buf, len);
	memcpy(buff+5+len," >>~/f.txt", strlen(" >>~/f.txt"));
	system(buff);
	return 0;
}


int main(int argv, char *args[])
{
	bt_info b_info;

	if (argv >=2)
		json_out = atoi(args[1]);
	if (argv >=3)
		cb_out = atoi(args[2]);

	if (json_out)
		cb_out = 0;

	bson_parser parser(get_n_bytes1, json_out);
	parser.start_parse(notify_bt_root, &b_info);

	return 0;
}