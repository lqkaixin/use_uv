#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <memory.h>
#include <errno.h>

int main()
{
	int skfd;
	struct sockaddr_in addr;
	skfd = socket(AF_INET, SOCK_STREAM, 0);
	memset(&addr, 0, sizeof(addr));
	
	printf("socket: %d\n", skfd);
	
	addr.sin_family = AF_INET;
	addr.sin_port = htons(13);
	addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	
	connect(skfd, (struct sockaddr *)&addr, sizeof(addr));
	
	printf("connect result: %d\n", errno);
	
	char buff[1024] = {0};
	int n = read(skfd, buff, sizeof(buff));
	
	
	printf("%d %s\n", n, buff);
	
	close(skfd);
	
	
	return 0;
}
