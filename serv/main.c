#include <stdio.h>
#include <stdlib.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

#include <stdint.h>

int main()
{
	printf("I will became a EAP server\n");

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
		uint8_t buf[1024];
		int bytes_read = recv(sock, buf, 1024, 0);
		for(int i = 0; i < bytes_read; printf("%02x", buf[i++]));
		printf("\n");
		close(sock);
	}

	return 0;
}
