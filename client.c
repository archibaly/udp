#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include "debug.h"
#include "common.h"

int main()
{
	struct sockaddr_in serv;
	char buff[BUFFSIZE];
	int sockfd;

	if ((sockfd = socket(PF_INET, SOCK_DGRAM, 0)) < 0) {
		debug("socket error: %s\n", strerror(errno));
		return -1;
	}
	
	bzero((char *)&serv, sizeof(serv));
	serv.sin_family = AF_INET;
	serv.sin_addr.s_addr = inet_addr("127.0.0.1");
	serv.sin_port = htons(SERV_PORT);

	strncpy(buff, "hello", sizeof(buff) - 1);
	for (;;) {
		if (sendto(sockfd, buff, strlen(buff) + 1, 0, (struct sockaddr *)&serv, sizeof(serv)) != strlen(buff) + 1) {
			debug("sendto error: %s\n", strerror(errno));
			return -1;
		}
		sleep(1);
	}

	return 0;
}
