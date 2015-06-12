//#define WINDOWS
#ifdef WINDOWS

#include <WINSOCK2.H> 
#include "windows.h"

#include <stdio.h>

#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <errno.h>

#include <ws2tcpip.h>

#pragma comment(lib, "ws2_32.lib") 

#define bzero(x, size)  memset(x, 0, size)

#else

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <errno.h>

#endif

#define LOG_DEBUG(fmt, ...) printf("[%s: %d] " fmt "\n", __FUNCTION__, __LINE__, ##__VA_ARGS__)

#define DNS_SVR "127.0.0.1"

#define P_SVR "127.0.0.1"
#define P_PORT 5300

#define DNS_HOST  0x01
#define DNS_CNAME 0x05


int socketfd;
struct sockaddr_in dest;

struct sockaddr_in request_src[256];
unsigned short request_idx[256];
int cur_idx;

static void send_dns_request(const char *dns_name);

static void parse_dns_response();

/**
 * Generate DNS question chunk
 */
static void generate_question(const char *dns_name
		, unsigned char *buf , int *len);

/**
 * Check whether the current byte is 
 * a dns pointer or a length
 */
static int is_pointer(int in);

/**
 * Parse data chunk into dns name
 * @param chunk The complete response chunk
 * @param ptr The pointer points to data
 * @param out This will be filled with dns name
 * @param len This will be filled with the length of dns name
 */
static void parse_dns_name(unsigned char *chunk , unsigned char *ptr
		, char *out , int *len);

static void proxy_handler();


int RUN_TEST = 0;

void proxy_server()
{
	memset(&dest ,0, sizeof(dest));
	dest.sin_family = AF_INET;
	dest.sin_port = htons(P_PORT);
	dest.sin_addr.s_addr = INADDR_ANY;
	
	int ret = 0;

	ret = bind(socketfd, (struct sockaddr*)&dest, sizeof(dest));
	
	LOG_DEBUG("bind_res: %d errno: %d", ret, errno);

	dest.sin_port = htons(53);
	dest.sin_addr.s_addr = inet_addr(DNS_SVR);
	while (1)
	{
		proxy_handler();
	}
}

int main(int argc , char *argv[]){

#ifdef WINDOWS
	 WSAData wsaData;
	WSAStartup(MAKEWORD(2,2),&wsaData);
#endif
	if(argc != 2){
		printf("Usage : %s \n" , argv[0]);
		exit(-1);
	}
	socketfd = socket(AF_INET , SOCK_DGRAM , 0);
	if(socketfd < 0){
		perror("create socket failed");
		exit(-1);
	}
	
	scanf("%d", &RUN_TEST);

	if (!RUN_TEST)proxy_server();

	printf(DNS_SVR "\n");
	bzero(&dest , sizeof(dest));
	dest.sin_family = AF_INET;
	dest.sin_port = htons(P_PORT);
	dest.sin_addr.s_addr = inet_addr(P_SVR);

	send_dns_request(argv[1]);

	parse_dns_response();

#ifdef WINDOWS
	WSACleanup();
#endif
	return 0;
}


static void proxy_handler(){

	unsigned char buf[1024];
	unsigned char *ptr = buf;
	struct sockaddr_in addr;
	char *src_ip;
	int n , i , flag , querys , answers;
	int type , ttl , datalen , len;
	char cname[128] , aname[128] , ip[20] , *cname_ptr,hname[128] = {0};
	unsigned char netip[4];
	size_t addr_len = sizeof(struct sockaddr_in);

	n = recvfrom(socketfd , (char *)buf , sizeof(buf) , 0
		, (struct sockaddr*)&addr , (int*)&addr_len);
		
	LOG_DEBUG("recv_len: %d", n);
	flag = ntohl(*((unsigned *)ptr));
	ptr += 4; /* move ptr to Questions */
	querys = ntohs(*((unsigned short*)ptr));
	ptr += 2; /* move ptr to Answer RRs */
	answers = ntohs(*((unsigned short*)ptr));
	ptr += 6; /* move ptr to Querys */
	/* move over Querys */
	for(i= 0 ; i < querys ; i ++){
		for(;;){
			len = 0;
			if (hname[0]==0)parse_dns_name(buf , ptr , hname , &len);
			int len = (int)ptr[0];
			ptr += (len + 1);
			if(len == 0)
				break;
		}
		ptr += 4;
	}
	
	LOG_DEBUG("recv_len: %d, flag: %x, querys: %d, answers: %d", n, flag, querys, answers);
	
	if (flag & 0x8000)  //response
	{
		LOG_DEBUG("recv a response");
		int tmp_idx = (unsigned char)buf[1];
		if (request_src[tmp_idx].sin_port != 0)
		{
			*(unsigned short*)buf = request_idx[tmp_idx];
			sendto(socketfd , (char*)buf , n, 0,
				 (struct sockaddr*)&request_src[tmp_idx] , sizeof(struct sockaddr));
			request_src[cur_idx].sin_port = 0;
		}
		return;

	}
	else //request
	{
		LOG_DEBUG("recv a request");
		if (strcmp(hname, "www.baidu.com") == 0)
		{
			buf[2] |= 0x80;
			buf[7] = 1;
			
			int t_len = n;
			
			buf[t_len++] = 0xc0;
			buf[t_len++] = 0x0c;
			
			
			buf[t_len++] = 0;
			buf[t_len++] = 1;
			buf[t_len++] = 0;
			buf[t_len++] = 1;
			*(int*)(buf+t_len) = htonl(350);
			t_len += 4;
			*(short*)(buf+t_len) = htons(4);
			t_len += 2;
			*(int*)(buf+t_len) = inet_addr("127.0.0.1");
			t_len += 4;
			
			sendto(socketfd , (char*)buf , t_len, 0,
				 (struct sockaddr*)&addr , sizeof(struct sockaddr));
		}
		else if (request_src[cur_idx].sin_port == 0)
		{
			memcpy(request_src+cur_idx, &addr, addr_len);
			request_idx[cur_idx] = *(unsigned short*)buf;
			buf[0] = 0xab;
			buf[1] = cur_idx;
			sendto(socketfd , (char*)buf , n, 0,
				 (struct sockaddr*)&dest , sizeof(struct sockaddr));
			cur_idx = (cur_idx + 1)%256;
		}
		return;
	}
}

static void parse_dns_response(){

	unsigned char buf[1024];
	unsigned char *ptr = buf;
	struct sockaddr_in addr;
	char *src_ip;
	int n , i , flag , querys , answers;
	int type , ttl , datalen , len;
	char cname[128] , aname[128] , ip[20] , *cname_ptr,hname[128] = {0};
	unsigned char netip[4];
	size_t addr_len = sizeof(struct sockaddr_in);

	n = recvfrom(socketfd , (char*)buf , sizeof(buf) , 0
		, (struct sockaddr*)&addr , (int*)&addr_len);
		
	LOG_DEBUG("recv_len: %d", n);
	flag = ntohl(*((unsigned *)ptr));
	ptr += 4; /* move ptr to Questions */
	querys = ntohs(*((unsigned short*)ptr));
	ptr += 2; /* move ptr to Answer RRs */
	answers = ntohs(*((unsigned short*)ptr));
	ptr += 6; /* move ptr to Querys */
	/* move over Querys */
	for(i= 0 ; i < querys ; i ++){
		for(;;){
			len = (int)ptr[0];
			ptr += (len + 1);
			if(len == 0)
				break;
		}
		ptr += 4;
	}
	
	LOG_DEBUG("-------------------------------");
#ifndef WINDOWS
	int tmp = str2hex(buf, n);
	LOG_DEBUG("resp_hex: %s", buf);
	hex2str(buf, tmp);
#endif
	/* now ptr points to Answers */
	for(i = 0 ; i < answers ; i ++){
		bzero(aname , sizeof(aname));
		len = 0;
		parse_dns_name(buf , ptr , aname , &len);
		ptr += 2; /* move ptr to Type*/
		type = htons(*((unsigned short*)ptr));
		ptr += 4; /* move ptr to Time to live */
		ttl = htonl(*((unsigned int*)ptr));
		ptr += 4; /* move ptr to Data lenth */
		datalen = ntohs(*((unsigned short*)ptr));
		ptr += 2; /* move ptr to Data*/
		
		LOG_DEBUG("type: %d datalen: %d", type, datalen);
		if(type == DNS_CNAME){
			bzero(cname , sizeof(cname));
			len = 0;
			parse_dns_name(buf , ptr , cname , &len);
			LOG_DEBUG("%s is an alias for %s" , aname , cname);
			ptr += datalen;
		}
		if(type == DNS_HOST){
			bzero(ip , sizeof(ip));
			if(datalen == 4){
				memcpy(netip , ptr , datalen);
				inet_ntop(AF_INET , netip , ip , sizeof(struct sockaddr));
				LOG_DEBUG("%s has address %s" , aname , ip);
				LOG_DEBUG("\tTime to live: %d minutes , %d seconds"
						, ttl / 60 , ttl % 60);
			}
			ptr += datalen;
		}

	}
	ptr += 2;
}

static void parse_dns_name(unsigned char *chunk
		, unsigned char *ptr , char *out , int *len){
	int n , alen , flag;
	char *pos = out + (*len);

	for(;;){
		flag = (int)ptr[0];
		if(flag == 0)
			break;
		if(is_pointer(flag)){
			n = (int)ptr[1];
			ptr = chunk + n;
			parse_dns_name(chunk , ptr , out , len);
			break;
		}else{
			ptr ++;
			memcpy(pos , ptr , flag);	
			pos += flag;
			ptr += flag;
			*len += flag;
			if((int)ptr[0] != 0){
				memcpy(pos , "." , 1);
				pos += 1;
				(*len) += 1;
			}
		}
	}

}

static int is_pointer(int in){
	return ((in & 0xc0) == 0xc0);
}

static void send_dns_request(const char *dns_name){

	unsigned char request[256];
	unsigned char *ptr = request;
	unsigned char question[128];
	int question_len;


	generate_question(dns_name , question , &question_len);

	*((unsigned short*)ptr) = htons(0xff00);
	ptr += 2;
	*((unsigned short*)ptr) = htons(0x0100);
	ptr += 2;
	*((unsigned short*)ptr) = htons(1);
	ptr += 2;
	*((unsigned short*)ptr) = 0;
	ptr += 2;
	*((unsigned short*)ptr) = 0;
	ptr += 2;
	*((unsigned short*)ptr) = 0;
	ptr += 2;
	memcpy(ptr , question , question_len);
	ptr += question_len;
	
#ifndef WINDOWS
	int tmp = str2hex(request, question_len + 12);
	LOG_DEBUG("resq_hex: %s", request);
	hex2str(request, tmp);
#endif

	sendto(socketfd , (char*)request , question_len + 12 , 0
	   , (struct sockaddr*)&dest , sizeof(struct sockaddr));
}

static void generate_question(const char *dns_name , unsigned char *buf , int *len){
	char *pos;
	unsigned char *ptr;
	int n;

	*len = 0;
	ptr = buf;	
	pos = (char*)dns_name; 
	for(;;){
		n = strlen(pos) - (strstr(pos , ".") ? strlen(strstr(pos , ".")) : 0);
		*ptr ++ = (unsigned char)n;
		memcpy(ptr , pos , n);
		*len += n + 1;
		ptr += n;
		if(!strstr(pos , ".")){
			*ptr = (unsigned char)0;
			ptr ++;
			*len += 1;
			break;
		}
		pos += n + 1;
	}
	*((unsigned short*)ptr) = htons(1);
	*len += 2;
	ptr += 2;
	*((unsigned short*)ptr) = htons(1);
	*len += 2;
}
