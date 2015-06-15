#include "log.h"

#include <string.h>
#include <stdlib.h>

#include "MD5.h"
#include "AES.h"

#include "shub_query.h"
#include "base64.h"

char *g_buf;
int g_len;


#define MAX_URL_LEN 1024
#define CID_SIZE 20
#define PEER_ID_SIZE 16
#define	PARTNER_ID_LEN					8
#define HUB_ENCODE_PADDING_LEN	16
#define QUERY_SERVER_RES		2001
#define HUB_CMD_HEADER_LEN		12
#define SHUB_PROTOCOL_VER		54

typedef unsigned long long _u64;
typedef unsigned _u32;
typedef unsigned short _u16;
typedef unsigned char _u8;

typedef long long _int64;
typedef int _int32;
typedef short _int16;
typedef char _int8;

typedef int BOOL;
#define TRUE 1
#define FALSE 0
#define SUCCESS 0


#define QUERY_BY_CID			0x01
#define QUERY_NO_BCID			0x02
#define QUERY_WITH_GCID			0x04

#define DEFAULT_URL "http://127.0.0.1"


typedef struct tagHUB_CMD_HEADER
{
	_u32	_version;
	_u32	_seq;
	_u32	_cmd_len;
}HUB_CMD_HEADER;

typedef struct tagQUERY_SERVER_RES_CMD		/*p2s protocol version is 50*/
{
	HUB_CMD_HEADER _header;
	_u32	_client_version;
	_u16	_compress;
	_u16	_cmd_type;
	_u8		_by_what;
	_u32	_url_or_gcid_size;
	char		_url_or_gcid[MAX_URL_LEN];
	_u32	_cid_size;						
	_u8		_cid[CID_SIZE];				/*cid are not use when query by url*/
	_u64	_file_size;					/*file size are not use when query by url*/
	_u32	_origin_url_size;
	char		_origin_url[MAX_URL_LEN];
	_u32	_max_server_res;
	_u8		_bonus_res_num;
	_u32	_peerid_size;
	char		_peerid[PEER_ID_SIZE + 1];
	_u32	_peer_report_ip;
	_u8		_peer_capability;
	_u32	_query_times_sn;
	_u8		_cid_query_type;			/*cid_query_type are not use when query by url*/
	_u32	_refer_url_size;
	char		_refer_url[MAX_URL_LEN];
	_u32	_partner_id_size;
	char		_partner_id[32];	//合作伙伴ID，最长长度为32
	_u32	_product_flag;
}QUERY_SERVER_RES_CMD;



_u8 g_peer_capability = 0;

void set_peer_capability(_u8* peer_capability, BOOL nated, BOOL support_intranet , BOOL same_nat, BOOL support_new_p2p, BOOL is_cdn,
						 BOOL support_ptl, BOOL support_new_udt)
{
	*peer_capability = 0;
	if(nated)
		*peer_capability = (*peer_capability) | 0x01;
	if(support_intranet)
		*peer_capability = (*peer_capability) | 0x02;
	if(same_nat)
		*peer_capability = (*peer_capability) | 0x04;
	if(support_new_p2p)
		*peer_capability = (*peer_capability) | 0x08;
	if(is_cdn)
		*peer_capability = (*peer_capability) | 0x10;
	if(support_ptl)
		*peer_capability = (*peer_capability) | 0x20;
	if(support_new_udt)
		*peer_capability = (*peer_capability) | 0x40;
}

_u8	get_peer_capability(void)
{
	// _u32 local_ip;
	if(g_peer_capability == 0)
	{
		// local_ip = sd_get_local_ip();
		set_peer_capability(&g_peer_capability, TRUE, TRUE, FALSE, TRUE, FALSE, TRUE, TRUE);
	}
	return g_peer_capability;	
}

_int32 get_peerid(char *buffer, _int32 bufsize)
{
	memset(buffer, 'A', PEER_ID_SIZE);
	return 0;
}

#define PARTNER_ID "20010000"

_int32 get_partner_id(char *buffer, _int32 bufsize)
{
#if defined(MOBILE_PHONE)
	snprintf(buffer, bufsize-1, "%d",0123);
#else
	strncpy(buffer, PARTNER_ID, bufsize);
#endif
	return SUCCESS;
}


_int32 get_product_flag(void)
{

/* product  is  relation to  the last character , 0x10000000 is  V*/
#if defined(MOBILE_PHONE)
      return(0x10000000);
#else
	#ifdef MACOS
      		return (0x1000000);    // MAC 迅雷
	#else
      return (0x2000);     
#endif
#endif

}

#define DEFAULT_SHUB_HOST_NAME "hub5sr.wap.sandai.net"
#define DEFAULT_SHUB_PORT 3076 //80  //

_int32 build_http_header(char* buffer, _u32* len, _u32 data_len, const char *host, _u16 port)
{
	*len = snprintf(buffer, *len,
				"POST http://%s:%u/ HTTP/1.1\r\n"
				"Host: %s:%u\r\n"
				"Content-Length: %u\r\n"
				"Content-Type: application/octet-stream\r\n"
				"Connection: Close\r\n"
				"User-Agent: Mozilla/4.0\r\n"
				"Accept: */*\r\n\r\n",
				host, port, host, port, data_len);

	return 0;
}

_int32 sd_set_int64_to_lt(char **buffer, _int32 *cur_buflen, _int64 value)
{
	if(*cur_buflen < sizeof(value))
		return -1;

	char *t = *buffer;
	*t++ = (value>> 0)&0xff;
	*t++ = (value>> 8)&0xff;
	*t++ = (value>>16)&0xff;
	*t++ = (value>>24)&0xff;
	*t++ = (value>>32)&0xff;
	*t++ = (value>>40)&0xff;
	*t++ = (value>>48)&0xff;
	*t++ = (value>>56)&0xff;
	*cur_buflen -= sizeof(value);
	*buffer = t;

	return SUCCESS;
}

_int32 sd_set_int32_to_lt(char **buffer, _int32 *cur_buflen, _int32 value)
{
	if(*cur_buflen < sizeof(value))
		return -1;

	char *t = *buffer;
	*t++ = (value>> 0)&0xff;
	*t++ = (value>> 8)&0xff;
	*t++ = (value>>16)&0xff;
	*t++ = (value>>24)&0xff;
	*cur_buflen -= sizeof(value);
	*buffer = t;

	return SUCCESS;
}

_int32 sd_set_int16_to_lt(char **buffer, _int32 *cur_buflen, _int16 value)
{
	if(*cur_buflen < sizeof(value))
		return -1;

	char *t = *buffer;
	*t++ = (value>> 0)&0xff;
	*t++ = (value>> 8)&0xff;
	*cur_buflen -= sizeof(value);
	*buffer = t;

	return SUCCESS;
}

_int32 sd_set_int8(char **buffer, _int32 *cur_buflen, _int8 value)
{
	if(*cur_buflen < sizeof(value))
		return -1;

	char *t = *buffer;
	*t++ = value;
	*cur_buflen -= sizeof(value);
	*buffer = t;

	return SUCCESS;
}

_int32 sd_set_bytes(char **buffer, _int32 *cur_buflen, char *src, _int32 src_len)
{
	if (*cur_buflen < src_len)
		return -1;
	memcpy(*buffer, src, src_len);
	*cur_buflen -= src_len;
	*buffer += src_len;
	return 0;
}

#define ENCRYPT_BLOCK_SIZE	16


_int32 aes_encrypt(char* buffer,_u32* len)
{
	_int32 nOutLen;
	_int32 pre_len;
	if (buffer == NULL)
	{
		return -1;
	}

	char b[16] = {0};
	MD5 md5;
	md5.md5_string(buffer, sizeof(_u32) * 2);
	md5.get_result(b);
	AES	aes((_u8*)b);

	pre_len = sizeof(_u32)*3;
	nOutLen = (*len - pre_len + 16)/16*16 + pre_len;
	memset(buffer + *len, nOutLen - *len, nOutLen - *len);

	aes.Cipher(buffer + pre_len, nOutLen - pre_len);

	char *tmp_buffer = buffer + sizeof(_u32) * 2;
	_int32 tmplen = 4;
    sd_set_int32_to_lt(&tmp_buffer, &tmplen, nOutLen- pre_len);

	*len = nOutLen;
	return SUCCESS;
}



_int32 build_query_server_res_cmd(char** buffer, _u32* len, QUERY_SERVER_RES_CMD* cmd)
{
	_int32 ret;
	char* tmp_buf;
	_int32 tmp_len;
	char http_header[1024] = {0};
	_u32 http_header_len = 1024;
	_u32 encode_len = 0;
	*len = HUB_CMD_HEADER_LEN + cmd->_header._cmd_len;
	encode_len = (cmd->_header._cmd_len + HUB_ENCODE_PADDING_LEN) /HUB_ENCODE_PADDING_LEN * HUB_ENCODE_PADDING_LEN + HUB_CMD_HEADER_LEN;
	ret = build_http_header(http_header, &http_header_len, encode_len, DEFAULT_SHUB_HOST_NAME, DEFAULT_SHUB_PORT);
	*buffer = (char *)malloc(*len + HUB_ENCODE_PADDING_LEN + http_header_len);
	
    memset(*buffer, 0, *len + HUB_ENCODE_PADDING_LEN + http_header_len);
	memcpy(*buffer, http_header, http_header_len);
	tmp_buf = *buffer + http_header_len;
	tmp_len = (_int32)*len;
	sd_set_int32_to_lt(&tmp_buf, &tmp_len, (_int32)cmd->_header._version);
	sd_set_int32_to_lt(&tmp_buf, &tmp_len, (_int32)cmd->_header._seq);
    sd_set_int32_to_lt(&tmp_buf, &tmp_len, (_int32)cmd->_header._cmd_len);
	sd_set_int32_to_lt(&tmp_buf, &tmp_len, (_int32)cmd->_client_version);
	sd_set_int16_to_lt(&tmp_buf, &tmp_len, (_int16)cmd->_compress);
	sd_set_int16_to_lt(&tmp_buf, &tmp_len, (_int16)cmd->_cmd_type);
	sd_set_int8(&tmp_buf, &tmp_len, (_int8)cmd->_by_what);
	sd_set_int32_to_lt(&tmp_buf, &tmp_len, (_int32)cmd->_url_or_gcid_size);
	sd_set_bytes(&tmp_buf, &tmp_len, cmd->_url_or_gcid, (_int32)cmd->_url_or_gcid_size);
	if(cmd->_by_what & QUERY_BY_CID)
	{
		sd_set_int32_to_lt(&tmp_buf, &tmp_len, (_int32)cmd->_cid_size);
		sd_set_bytes(&tmp_buf, &tmp_len, (char*)cmd->_cid, (_int32)cmd->_cid_size);
	}
	else
	{
		_u32 len = 0;
		sd_set_int32_to_lt(&tmp_buf, &tmp_len, (_int32)len);
	}
	sd_set_int64_to_lt(&tmp_buf, &tmp_len, (_int64)cmd->_file_size);
	sd_set_int32_to_lt(&tmp_buf, &tmp_len, (_int32)cmd->_origin_url_size);
	sd_set_bytes(&tmp_buf, &tmp_len, cmd->_origin_url, cmd->_origin_url_size);
	sd_set_int32_to_lt(&tmp_buf, &tmp_len, (_int32)cmd->_max_server_res);
	sd_set_int8(&tmp_buf, &tmp_len, (_int8)cmd->_bonus_res_num);
	sd_set_int32_to_lt(&tmp_buf, &tmp_len, (_int32)cmd->_peerid_size);
	sd_set_bytes(&tmp_buf, &tmp_len, cmd->_peerid, (_int32)cmd->_peerid_size);
	sd_set_bytes(&tmp_buf, &tmp_len, (char* )&cmd->_peer_report_ip, sizeof(_u32));
	sd_set_int8(&tmp_buf, &tmp_len, (_int8)cmd->_peer_capability);
	sd_set_int32_to_lt(&tmp_buf, &tmp_len, (_int32)cmd->_query_times_sn);
	sd_set_int8(&tmp_buf, &tmp_len, (_int8)cmd->_cid_query_type);
	sd_set_int32_to_lt(&tmp_buf, &tmp_len, (_int32)cmd->_refer_url_size);
	sd_set_bytes(&tmp_buf, &tmp_len, cmd->_refer_url, (_int32)cmd->_refer_url_size);
	sd_set_int32_to_lt(&tmp_buf, &tmp_len, (_int32)cmd->_partner_id_size);
	sd_set_bytes(&tmp_buf, &tmp_len, cmd->_partner_id, cmd->_partner_id_size);
	ret = sd_set_int32_to_lt(&tmp_buf, &tmp_len, (_int32)cmd->_product_flag);

	ret = aes_encrypt(*buffer + http_header_len, len);

	*len += http_header_len;

	return SUCCESS;
}

_int32 res_query_shub_by_cid(const _u8* cid, _u64 file_size, BOOL is_gcid, const char* url_or_gcid,
							 BOOL need_bcid,_int32 max_server_res, _int32 bonus_res_num, _int32 query_times_sn, _u8 cid_query_type)
{	
	_int32 ret;
	/*fill request package*/
	QUERY_SERVER_RES_CMD cmd;
// #ifdef _LOGGER
// {
// 		char cid_t[41],gcid_t[41];
// 		sd_memset(cid_t,0x00,41);
// 		sd_memset(gcid_t,0x00,41);
// 		str2hex((char*)cid, CID_SIZE, cid_t, 40);
// 		cid_t[40] = '\0';
// 		if(is_gcid)
// 		{
// 			str2hex((char*)url_or_gcid, CID_SIZE, gcid_t, 40);
// 			gcid_t[40] = '\0';
// 		}
// 		LOG_ERROR("res_query_shub_by_cid, cid = %s, gcid = %s, is_gcid,=%d,file_size = %llu", cid_t, gcid_t,is_gcid,file_size);
// #ifdef IPAD_KANKAN
// 		printf("\nres_query_shub_by_cid, cid = %s, gcid = %s, is_gcid,=%d,file_size = %llu\n", cid_t, gcid_t,is_gcid,file_size);
// #endif
// }
// #endif
	LOG_DEBUG("res_query_shub_by_cid, user_data = 0x%x.",0);
	memset(&cmd, 0, sizeof(QUERY_SERVER_RES_CMD));

	cmd._header._version = SHUB_PROTOCOL_VER;
	cmd._header._seq = 123;

	cmd._by_what = QUERY_BY_CID;
	if(!need_bcid)
		cmd._by_what |= QUERY_NO_BCID;
	if(is_gcid)
	{
		cmd._by_what |= QUERY_WITH_GCID;
		cmd._url_or_gcid_size = CID_SIZE;
		memcpy(cmd._url_or_gcid, url_or_gcid, CID_SIZE);
	}
	else
	{
		cmd._url_or_gcid_size = strlen(url_or_gcid);
		memcpy(cmd._url_or_gcid, url_or_gcid, cmd._url_or_gcid_size);
	}

	cmd._cid_size = CID_SIZE;
	memcpy(cmd._cid, cid, CID_SIZE);
	cmd._origin_url_size = strlen(DEFAULT_URL);
	memcpy(cmd._origin_url, DEFAULT_URL, cmd._origin_url_size + 1);
	cmd._refer_url_size = strlen(DEFAULT_URL);
	memcpy(cmd._refer_url, DEFAULT_URL, cmd._refer_url_size);
	cmd._partner_id_size = PARTNER_ID_LEN;
	get_partner_id(cmd._partner_id, PARTNER_ID_LEN);
	cmd._peerid_size = PEER_ID_SIZE;
	get_peerid(cmd._peerid, PEER_ID_SIZE + 1);

	cmd._cmd_type = QUERY_SERVER_RES;
	cmd._file_size = file_size;
	cmd._max_server_res = max_server_res;
	cmd._bonus_res_num = bonus_res_num;
	cmd._query_times_sn = query_times_sn;
	cmd._cid_query_type = cid_query_type;
	cmd._peer_report_ip = 0;//sd_get_local_ip();
	cmd._peer_capability = get_peer_capability();
	cmd._product_flag = get_product_flag();	

	cmd._header._cmd_len = 60 + 
				cmd._url_or_gcid_size +
				cmd._cid_size +
				cmd._origin_url_size +
				cmd._peerid_size +
				cmd._refer_url_size +
				cmd._partner_id_size;
	/*build command*/
	char* buffer = NULL;
	_u32  len = 0;
	ret = build_query_server_res_cmd(&buffer, &len, &cmd);

	g_buf = buffer;
	g_len = len;
	// CHECK_VALUE(ret);
	// return res_query_commit_cmd(&g_shub, QUERY_SERVER_RES, buffer, len, (void*)handler, user_data, query_res_cmd._header._seq,TRUE);			//commit a request
	return 0;
}


_int32 res_query_shub_by_url(const char* url, const char* refer_url,
							BOOL need_bcid, _int32 max_server_res, _int32 bonus_res_num,
							_int32 query_times_sn,BOOL not_add_res)
{
	_int32 ret;
	/*fill request package*/
	QUERY_SERVER_RES_CMD cmd;
	LOG_DEBUG("res_query_shub_by_url, user_data = 0x%x", 0);
	memset(&cmd, 0, sizeof(cmd));

	cmd._header._version = SHUB_PROTOCOL_VER;
	cmd._header._seq = 123;

	cmd._by_what = 0x00;
	if(!need_bcid)
		cmd._by_what |= QUERY_NO_BCID;

	cmd._url_or_gcid_size = strlen(url);
	memcpy(cmd._url_or_gcid, url, cmd._url_or_gcid_size);
	cmd._origin_url_size = strlen(url);
	memcpy(cmd._origin_url, url, cmd._origin_url_size);

	cmd._refer_url_size = strlen(refer_url);
	memcpy(cmd._refer_url, refer_url, cmd._refer_url_size);

	cmd._peerid_size = PEER_ID_SIZE;
	get_peerid(cmd._peerid, PEER_ID_SIZE + 1);
	cmd._partner_id_size = PARTNER_ID_LEN;
	get_partner_id(cmd._partner_id, PARTNER_ID_LEN);

	cmd._cmd_type = QUERY_SERVER_RES;
	cmd._max_server_res = max_server_res;
	cmd._bonus_res_num = bonus_res_num;
	cmd._query_times_sn = query_times_sn;
	cmd._peer_report_ip = 0;//sd_get_local_ip();
	cmd._peer_capability = get_peer_capability();
	cmd._product_flag = get_product_flag();

	cmd._header._cmd_len = 60 + 
				cmd._url_or_gcid_size +
				cmd._cid_size +
				cmd._origin_url_size +
				cmd._peerid_size +
				cmd._refer_url_size +
				cmd._partner_id_size;
	/*build command*/
	char* buffer = NULL;
	_u32  len = 0;
	ret = build_query_server_res_cmd(&buffer, &len, &cmd);

	LOG_DEBUG("build cmd res: %d", ret);

	g_buf = buffer;
	g_len = len;
	// CHECK_VALUE(ret);
	// return res_query_commit_cmd(&g_shub, QUERY_SERVER_RES, buffer, len, (void*)handler, user_data, query_res_cmd._header._seq,not_add_res);
	return 0;
}

int test_shub_query()
{
	BOOL need_bcid = TRUE;
	_int32 max_server_res = 1, bonus_res_num = 20;
	
	char url[] = "";
	char ref_url[] = "";

	//res_query_shub_by_url(url, ref_url, need_bcid, max_server_res, bonus_res_num, 231, 1/*,BOOL not_add_res*/);

	_u8 tcid[CID_SIZE] = {0}, gcid[CID_SIZE] = {0};
	BOOL is_gcid = 0;

	hex2str("5339E62BE890ED1852DCB8BD4C01C6C2D9EDF9B5", 40, (char *)tcid, 20);

	res_query_shub_by_cid(tcid, 34475476, is_gcid, "gvod://10.0.0.8:8080/5339E62BE890ED1852DCB8BD4C01C6C2D9EDF9B5/34475476/[激情] 苍井空 教师.rmvb", need_bcid, max_server_res,
			bonus_res_num, 123, 0);

	LOG_DEBUG("%d %s", g_len, g_buf);
	return 0;
}

shub_queryer::shub_queryer():dev(this)
{
	test_shub_query();
    dev.connect(DEFAULT_SHUB_HOST_NAME, DEFAULT_SHUB_PORT);
}

void shub_queryer::conn_cb(){dev.send(g_buf, g_len);}
void shub_queryer::header_cb(){}
void shub_queryer::body_cb(const char *data, int len){ LOG_DEBUG("%d", len);}
