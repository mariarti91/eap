#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "eap.h"

#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

void sendData(const eap_package* pack)
{
	printf("EAP package code = %02x\n", pack->code);
	printf("EAP package identifier = %02x\n", pack->identifier);
	printf("EAP package length = %04x\n", pack->length);

	uint16_t size = pack->length;
	uint8_t *buf = malloc(size);
	memcpy(buf, pack, 2);
	size = htons(size);
	memcpy(&buf[2], &size, 2);
	size = ntohs(size);
	memcpy(&buf[4], pack->data, size - 4);

	for(uint8_t i = 0; i < size; i++)
		printf("%02x", buf[i]);
	printf("\n");

	int sock = socket(AF_INET, SOCK_STREAM, 0);
	if(sock < 0) {printf("Could not create socket\n"); return;}

	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(9090);
	addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
	if(connect(sock, (struct sockaddr *)&addr, sizeof(addr)) < 0)
	{
		printf("Could not connect to hostn\n");
		return;
	}
	send(sock, buf, size, 0);
	free(buf);
	close(sock);
}

int main()
{
	printf("I will became a EAP peer\n");
	eap_package eapPack;
	eapPack.code = EAP_REQUEST;
	eapPack.identifier = 0xee;
	eapPack.length = 8;
	eapPack.data = malloc(4);
	memset(eapPack.data, 0xdd, 4);

	sendData(&eapPack);
	return 0;
}
