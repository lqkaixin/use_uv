
#include <stdio.h>
#include <sys/socket.h>
#include <memory.h>
#include <netinet/in.h>
#include <errno.h>
#include <time.h>

int main()
{
	printf("server 1\n");
	
	int lfd, cfd;
	struct sockaddr_in addr;
	char buff[100] = {0};
	time_t ticks;
	
	printf("server 2\n");
	
	lfd = socket(AF_INET, SOCK_STREAM, 0);
	memset(&addr, 0, sizeof(addr));
	
	printf("server 3\n");
	
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = INADDR_ANY;
	addr.sin_port = htons(13);
	
	printf("server 4\n");
	
	bind(lfd, (struct sockaddr *)&addr, sizeof(addr));
	printf("server bind result: %d\n", errno);
	
	listen(lfd, 2);
	printf("server listen result: %d\n", errno);
	
	while(1)
	{
		cfd = accept(lfd, NULL, NULL);
		ticks = time(NULL);
		snprintf(buff, sizeof(buff), "%s\r\n", ctime(&ticks));
		printf("server: %s\n", buff);
		send(cfd, buff, strlen(buff), 0);
		
		close(cfd);
	}
}
