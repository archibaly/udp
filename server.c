#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include "debug.h"
#include "common.h"

static int count;
static void recvfrom_int(int signo);

int main()
{
	int sockfd;
	sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	if (sockfd == -1) {
		debug("socket error: %s\n", strerror(errno));
		return -1;
	}

	struct sockaddr_in servaddr;
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(SERV_PORT);
	
	if (bind(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
		debug("bind error: %s", strerror(errno));
		return -1;
	}

	signal(SIGINT, recvfrom_int);

	int n;
	char addr[INETADDR_LEN];
	char buffer[BUFFSIZE];
	struct sockaddr_in cliaddr;
	socklen_t clilen = sizeof(cliaddr);
	for (;;) {
		n = recvfrom(sockfd, buffer, sizeof(buffer), 0, (struct sockaddr *)&cliaddr, &clilen);
		if (n == 0) {	/* the peer has performed an orderly shutdown */
			debug("the peer closed\n");
		} else if (n < 0) {
			debug("recvfrom error: %s\n", strerror(errno));
			exit(1);
		} else {
			buffer[n] = '\0';
			printf("receive from %s: %s\n", inet_ntop(AF_INET, &cliaddr.sin_addr, addr, clilen), buffer);
		}
		count++;
	}
	return 0;
}

static void recvfrom_int(int signo)
{
	printf("received %d datagrams\n", count);
	exit(0);
}
