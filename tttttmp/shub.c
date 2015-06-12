#include <memory.h>
#include <string.h>


typedef unsigned int uint32;
typedef unsigned short uint16;
typedef unsigned char uint8;
typedef unsigned long long uint64;

typedef int int32;
typedef short int16;
typedef char int8;
typedef long long int64;

typedef unsigned int u32;
typedef unsigned short u16;
typedef unsigned char u8;
typedef unsigned long long u64;

typedef int BOOL;


#define TRUE 1
#define FALSE 0

#ifndef NULL
#define NULL 0
#endif

#define LOG_DEBUG(...)

#if 0
const int MAX_URL_LEN = 1024;
const int CID_SIZE = 20;
const int PEER_ID_SIZE = 16;
#else
#define MAX_URL_LEN  1024
#define CID_SIZE  20
#define PEER_ID_SIZE  16

#endif



#define QUERY_BY_CID			0x01  
#define QUERY_NO_BCID			0x02
#define QUERY_WITH_GCID			0x04

#define	PARTNER_ID_LEN			8

#define SHUB_PROTOCOL_VER		54

#define DEFAULT_URL		"http://127.0.0.1"

#define HUB_ENCODE_PADDING_LEN 16


int32 g_shub_seq = 0;

#define DEFAULT_SHUB_HOST_NAME "hub5sr.wap.sandai.net"
#define DEFAULT_SHUB_PORT 3076 //80


u8* get_peerid() {return NULL;}
int get_peer_capability(){ return 0; }
u8* get_partner_id(){return NULL;}
int get_product_flag(){return 0;}


typedef struct tagHUB_CMD_HEADER
{
	u32	_version;
	u32	_seq;
	u32	_cmd_len;
}HUB_CMD_HEADER;

typedef struct tagQUERY_SERVER_RES_CMD		/*p2s protocol version is 50*/
{
	HUB_CMD_HEADER	_header;
	u32				_client_version;
	u16				_compress;
	u16				_cmd_type;
	u8				_by_what;
	u32				_url_or_gcid_size;
	char			_url_or_gcid[MAX_URL_LEN];
	u32				_cid_size;
	u8				_cid[CID_SIZE];				/*cid are not use when query by url*/
	u64				_file_size;					/*file size are not use when query by url*/
	u32				_origin_url_size;
	char			_origin_url[MAX_URL_LEN];
	u32				_max_server_res;
	u8				_bonus_res_num;
	u32				_peerid_size;
	char			_peerid[PEER_ID_SIZE + 1];
	u32				_peer_report_ip;
	u8				_peer_capability;
	u32				_query_times_sn;
	u8				_cid_query_type;			/*cid_query_type are not use when query by url*/
	u32				_refer_url_size;
	char			_refer_url[MAX_URL_LEN];
	u32				_partner_id_size;
	char			_partner_id[32];	//合作伙伴ID，最长长度为32
	u32				_product_flag;
}QUERY_SERVER_RES_CMD;

int build_query_server_res_cmd(char** buffer, u32* len, QUERY_SERVER_RES_CMD* cmd);


int res_query_shub_by_cid(const u8* cid, u64 file_size, BOOL is_gcid, const u8* url_or_gcid,
							 BOOL need_bcid, int32 max_server_res, int32 bonus_res_num, int32 query_times_sn, u8 cid_query_type)
{	
	char* buffer = NULL;
	u32  len = 0;
	int32 ret;
	/*fill request package*/
	QUERY_SERVER_RES_CMD query_res_cmd = {{0}};
#ifdef _DEBUG
	{
		char cid_t[41],gcid_t[41];
		memset(cid_t,0x00,41);
		memset(gcid_t,0x00,41);
		str2hex((char*)cid, CID_SIZE, cid_t, 40);
		cid_t[40] = '\0';
		if(is_gcid)
		{
			str2hex((char*)url_or_gcid, CID_SIZE, gcid_t, 40);
			gcid_t[40] = '\0';
		}
		LOG_ERROR("res_query_shub_by_cid, cid = %s, gcid = %s, is_gcid,=%d,file_size = %llu", cid_t, gcid_t,is_gcid,file_size);
#ifdef IPAD_KANKAN
		printf("\nres_query_shub_by_cid, cid = %s, gcid = %s, is_gcid,=%d,file_size = %llu\n", cid_t, gcid_t,is_gcid,file_size);
#endif
	}
#endif

	query_res_cmd._by_what = QUERY_BY_CID;
	if(!need_bcid)
		query_res_cmd._by_what |= QUERY_NO_BCID;
	if(is_gcid)
	{
		query_res_cmd._by_what |= QUERY_WITH_GCID;
		query_res_cmd._url_or_gcid_size = CID_SIZE;
		memcpy(query_res_cmd._url_or_gcid, url_or_gcid, CID_SIZE);
	}
	else
	{
		query_res_cmd._url_or_gcid_size = strlen((char*)url_or_gcid);
		memcpy(query_res_cmd._url_or_gcid, url_or_gcid, query_res_cmd._url_or_gcid_size + 1);
	}
	
	query_res_cmd._cid_size = CID_SIZE;
	memcpy(query_res_cmd._cid, cid, CID_SIZE);
	
	query_res_cmd._file_size = file_size;
	
	query_res_cmd._origin_url_size = strlen(DEFAULT_URL);
	memcpy(query_res_cmd._origin_url, DEFAULT_URL, query_res_cmd._origin_url_size + 1);
	
	query_res_cmd._max_server_res = max_server_res;
	query_res_cmd._bonus_res_num = bonus_res_num;
	
	query_res_cmd._peerid_size = PEER_ID_SIZE;
	memcpy(query_res_cmd._peerid, get_peerid(), PEER_ID_SIZE);
	// get_peerid(query_res_cmd._peerid, PEER_ID_SIZE + 1);
	
	query_res_cmd._peer_capability = get_peer_capability();
	query_res_cmd._query_times_sn = query_times_sn;
	query_res_cmd._cid_query_type = cid_query_type;
	
	query_res_cmd._refer_url_size = strlen(DEFAULT_URL);
	memcpy(query_res_cmd._refer_url, DEFAULT_URL, query_res_cmd._refer_url_size);
	
	query_res_cmd._partner_id_size = PARTNER_ID_LEN;
	memcpy(query_res_cmd._partner_id, get_partner_id(), PARTNER_ID_LEN);
	// get_partner_id(query_res_cmd._partner_id, PARTNER_ID_LEN);
	
	query_res_cmd._product_flag = get_product_flag();	
	
	
	QUERY_SERVER_RES_CMD *cmd = &query_res_cmd;
	
	cmd->_header._version = SHUB_PROTOCOL_VER;
	cmd->_header._seq = g_shub_seq++; 
	cmd->_header._cmd_len = 60 + cmd->_url_or_gcid_size + cmd->_cid_size + cmd->_origin_url_size + cmd->_peerid_size + cmd->_refer_url_size + cmd->_partner_id_size;
	/*build command*/
	ret = build_query_server_res_cmd(&buffer, &len, &query_res_cmd);

	//return res_query_commit_cmd(&g_shub, QUERY_SERVER_RES, buffer, len, (void*)handler, user_data, query_res_cmd._header._seq,TRUE);			//commit a request
	
	return 0;
}

#define QUERY_SERVER_RES		2001
#define HUB_CMD_HEADER_LEN		12


int32 build_query_server_res_cmd(char** buffer, u32* len, QUERY_SERVER_RES_CMD* cmd)
{
	int ret;
	char* tmp_buf;
	int tmp_len;
	char http_header[1024] = {0};
	u32 http_header_len = 1024;
	u32 encode_len = 0;
	cmd->_header._version = SHUB_PROTOCOL_VER;
	cmd->_header._seq = g_shub_seq++; 
	cmd->_header._cmd_len = 60 + cmd->_url_or_gcid_size + cmd->_cid_size + cmd->_origin_url_size + cmd->_peerid_size + cmd->_refer_url_size + cmd->_partner_id_size;
	*len = HUB_CMD_HEADER_LEN + cmd->_header._cmd_len;
	encode_len = (cmd->_header._cmd_len + HUB_ENCODE_PADDING_LEN) /HUB_ENCODE_PADDING_LEN * HUB_ENCODE_PADDING_LEN + HUB_CMD_HEADER_LEN;
	ret = build_http_header(http_header, &http_header_len, encode_len,device->_hub_type);

	cmd->_cmd_type = QUERY_SERVER_RES;
	ret = sd_malloc(*len + HUB_ENCODE_PADDING_LEN + http_header_len, (void**)buffer);

    memset(*buffer, 0, *len + HUB_ENCODE_PADDING_LEN + http_header_len);
	memcpy(*buffer, http_header, http_header_len);
	tmp_buf = *buffer + http_header_len;
	tmp_len = (int32)*len;
	sd_set_int32_to_lt(&tmp_buf, &tmp_len, (int32)cmd->_header._version);
	sd_set_int32_to_lt(&tmp_buf, &tmp_len, (int32)cmd->_header._seq);
    sd_set_int32_to_lt(&tmp_buf, &tmp_len, (int32)cmd->_header._cmd_len);
	sd_set_int32_to_lt(&tmp_buf, &tmp_len, (int32)cmd->_client_version);
	sd_set_int16_to_lt(&tmp_buf, &tmp_len, (int16)cmd->_compress);
	sd_set_int16_to_lt(&tmp_buf, &tmp_len, (int16)cmd->_cmd_type);
	sd_set_int8(&tmp_buf, &tmp_len, (int8)cmd->_by_what);
	sd_set_int32_to_lt(&tmp_buf, &tmp_len, (int32)cmd->_url_or_gcid_size);
	sd_set_bytes(&tmp_buf, &tmp_len, cmd->_url_or_gcid, (int32)cmd->_url_or_gcid_size);
	if(cmd->_by_what & QUERY_BY_CID)
	{
		sd_set_int32_to_lt(&tmp_buf, &tmp_len, (int32)cmd->_cid_size);
		sd_set_bytes(&tmp_buf, &tmp_len, (char*)cmd->_cid, (int32)cmd->_cid_size);
	}
	else
	{
		u32 len = 0;
		sd_set_int32_to_lt(&tmp_buf, &tmp_len, (int32)len);
	}
	sd_set_int64_to_lt(&tmp_buf, &tmp_len, (int64)cmd->_file_size);
	sd_set_int32_to_lt(&tmp_buf, &tmp_len, (int32)cmd->_origin_url_size);
	sd_set_bytes(&tmp_buf, &tmp_len, cmd->_origin_url, cmd->_origin_url_size);
	sd_set_int32_to_lt(&tmp_buf, &tmp_len, (int32)cmd->_max_server_res);
	sd_set_int8(&tmp_buf, &tmp_len, (int8)cmd->_bonus_res_num);
	sd_set_int32_to_lt(&tmp_buf, &tmp_len, (int32)cmd->_peerid_size);
	sd_set_bytes(&tmp_buf, &tmp_len, cmd->_peerid, (int32)cmd->_peerid_size);
	sd_set_bytes(&tmp_buf, &tmp_len, (char* )&cmd->_peer_report_ip, sizeof(u32));
	sd_set_int8(&tmp_buf, &tmp_len, (int8)cmd->_peer_capability);
	sd_set_int32_to_lt(&tmp_buf, &tmp_len, (int32)cmd->_query_times_sn);
	sd_set_int8(&tmp_buf, &tmp_len, (int8)cmd->_cid_query_type);
	sd_set_int32_to_lt(&tmp_buf, &tmp_len, (int32)cmd->_refer_url_size);
	sd_set_bytes(&tmp_buf, &tmp_len, cmd->_refer_url, (int32)cmd->_refer_url_size);
	sd_set_int32_to_lt(&tmp_buf, &tmp_len, (int32)cmd->_partner_id_size);
	sd_set_bytes(&tmp_buf, &tmp_len, cmd->_partner_id, cmd->_partner_id_size);
	ret = sd_set_int32_to_lt(&tmp_buf, &tmp_len, (int32)cmd->_product_flag);
    sd_assert(ret==SUCCESS);
#ifndef AMLOS	
	ret = aes_encrypt(*buffer + http_header_len, len);
#else
	ret = aes_encrypt_ext(*buffer + http_header_len, len);
#endif

	*len += http_header_len;
	return SUCCESS;
}

int main()
{
	u8 cid[] = "";
	u64 file_size = 0;
	BOOL is_gcid = TRUE;
	u8 url_or_gcid[] = "TRUE";
	BOOL need_bcid = TRUE;
	int32 max_server_res = 10;
	int32 bonus_res_num = 10;
	int32 query_times_sn = 0;
	u8 cid_query_type = 0;

	res_query_shub_by_cid(cid, file_size, is_gcid, url_or_gcid, need_bcid,
			max_server_res, bonus_res_num, query_times_sn, cid_query_type);
	return 0;
}
