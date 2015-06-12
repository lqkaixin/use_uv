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


#define P_SVR "127.0.0.1"
#define P_PORT 5300


int socketfd_svr;
struct sockaddr_in svr;

int socketfd_clt;
struct sockaddr_in clt;

int RUN_TEST = 0;

void socket_server()
{
	memset(&svr ,0, sizeof(svr));
	svr.sin_family = AF_INET;
	svr.sin_port = htons(P_PORT);
	svr.sin_addr.s_addr = INADDR_ANY;
	
	int ret = 0;

	socketfd_svr = socket(AF_INET , SOCK_STREAM , 0);
	ret = bind(socketfd_svr, (struct sockaddr*)&svr, sizeof(svr));
	ret = recv(socketfd_clt, buff, 1024, 0);
	LOG_DEBUG("bind res: %d, errno: %d", ret, errno);

	ret = listen(socketfd_svr, 0);
	
	LOG_DEBUG("listen res: %d, errno: %d", ret, errno);
	
	while (1)
	{
		char buff[1024] = {0};

		int tmp = sizeof(clt);

		socketfd_clt = accept(socketfd_svr, (struct sockaddr*)&clt, &tmp);
		
		LOG_DEBUG("accept res: %d, errno: %d", socketfd_clt, errno);
		
		if (socketfd_clt == -1)
			break;
		
		ret = send(socketfd_clt, "hello i'm server!\n", strlen("hello i'm server!\n"), 0);
		
		LOG_DEBUG("send res: %d", ret);
		
		ret = recv(socketfd_clt, buff, 1024, 0);
		
		LOG_DEBUG("recv res: %d", ret);
		
		sleep(5);
		
		ret = recv(socketfd_clt, buff, 1024, 0);
		
		LOG_DEBUG("recv res: %d", ret);
		
		sleep(15);
		
		LOG_DEBUG("now close");
		
		close(socketfd_clt);
		while(1)
		{
			LOG_DEBUG("timeout");
			sleep(10);
		}
		
		printf("%s", buff);
	}
}

void socket_client()
{
	memset(&svr ,0, sizeof(svr));
	svr.sin_family = AF_INET;
	svr.sin_port = htons(P_PORT);
	svr.sin_addr.s_addr = inet_addr(P_SVR);
	
	int ret = 0;
	socketfd_clt = socket(AF_INET , SOCK_STREAM , 0);
	ret = connect(socketfd_clt, (struct sockaddr*)&svr, sizeof(svr));
	
	LOG_DEBUG("connect res: %d, errno: %d", ret, errno);

	while (1)
	{
	
		char buff[1024] = {0};
		
		ret = send(socketfd_clt, "hello i'm client!\n", strlen("hello i'm client!\n"), 0);
		
		LOG_DEBUG("send res: %d", ret);
		
		ret = recv(socketfd_clt, buff, 1024, 0);
		
		LOG_DEBUG("recv res: %d", ret);
		
		if (ret <= 0)
		{while(1){
		ret = recv(socketfd_clt, buff, 1024, 0);
			LOG_DEBUG("timeout recv res: %d", ret);
			sleep(10);}
		}
		
		printf("%s", buff);
	}
	LOG_DEBUG("out while");
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
	
	//scanf("%d", &RUN_TEST);

	if (argv[1][0]=='1')socket_server();
	else socket_client();


#ifdef WINDOWS
	WSACleanup();
#endif
	return 0;
}
