#include <stdio.h>
#include <stdlib.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <sys/time.h>
#include <stdint.h>

#include "eap.h"

int main()
{
	printf("I will became a EAP peer\n");

	int listener = socket(AF_INET, SOCK_STREAM, 0);
	if(listener < 0) {printf("Could not create socket\n"); return -1;}

	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(9090);
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	if(bind(listener, (struct sockaddr *)&addr, sizeof(addr)) < 0)
	{
		printf("Could not bind socket\n");
		return -2;
	}

	listen(listener, 1);

	while(1)
	{
		int sock = accept(listener, NULL, NULL);
		if(sock < 0) {printf("Could not get socket\n"); return -3;}

		struct timeval timeout;
		timeout.tv_sec = 10;
		timeout.tv_usec = 0;
		if(setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, (const char*)&timeout, sizeof(struct timeval)) < 0)
		{
			printf("Could not set up timeout\n");
		}

		uint8_t buf[1024];
		int bytes_read = recv(sock, buf, 1024, 0);
		if(bytes_read <= 0)
		{
			printf("Time out!\n");
			break;
		}
		for(int i = 0; i < bytes_read; printf("%02x", buf[i++]));
		printf("\n");
		eap_package* pack = malloc(0);
		if(dataToEap(&buf[0], bytes_read, pack) < 0)
			printf("data is not EAP package\n");
		else
		{
			printf("EAP package code = %02x\n", pack->code);
			printf("EAP package identifier = %02x\n", pack->identifier);
			printf("EAP package length = %04x\n", pack->length);
			for(int i = 0; i < pack->length - 4; printf("%02x", pack->data[i++]));
			printf("\n");
		}
		close(sock);
	}

	return 0;
}
