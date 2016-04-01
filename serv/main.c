#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "eap.h"

#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <sys/time.h>

#define RECVBUF_SIZE 1000
uint8_t buf_for_recv[RECVBUF_SIZE];

void sendData(const eap_package* pack)
{
	printf("EAP package code = %02x\n", pack->code);
	printf("EAP package identifier = %02x\n", pack->identifier);
	printf("EAP package length = %04x\n", pack->length);

	uint8_t* buf = malloc(0);
	int size = eapToData(pack, buf);
	for(uint8_t i = 0; i < size; printf("%02x", buf[i++]));
	printf("\n");

	int sock = socket(AF_INET, SOCK_STREAM, 0);
	if(sock < 0) {printf("Could not create socket\n"); return;}

	struct timeval timeout;
	timeout.tv_sec = 5;
	timeout.tv_usec = 0;

	if(setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, (const char*)&timeout, sizeof(struct timeval)) < 0)
	{
		printf("Could not set up timeout\n");
	}

	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(9090);
	addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
	if(connect(sock, (struct sockaddr *)&addr, sizeof(addr)) < 0)
	{
		printf("Could not connect to hostn\n");
		return;
	}

	do{
		send(sock, buf, size, 0);
		int bytes_read = recv(sock, buf_for_recv, RECVBUF_SIZE, 0);
		if(bytes_read <= 0) {printf("Resending\n");continue;}
		for(int i = 0; i < bytes_read; printf("%02x", buf_for_recv[i++]));
		printf("\n");
		eap_package *pack = malloc(0);
		if(dataToEap(&buf[0], bytes_read, pack) < 0){printf("It is not EAP package\n"); continue;}
		
		printf("EAP package code = %02x\n", pack->code);
		printf("EAP package identifier = %02x\n", pack->identifier);
		printf("EAP package length = %04x\n", pack->length);
		printf("EAP data: ");
		for(int i = 0; i < pack->length - 4; printf("%02x", pack->data[i++]));
		printf("\n");
		break;
	}while(1);
	free(buf);
	close(sock);
}

int main()
{
	printf("I will became a EAP server\n");
	eap_package eapPack;
	eapPack.code = EAP_REQUEST;
	eapPack.identifier = 0xee;
	eapPack.length = 8;
	eapPack.data = malloc(4);
	memset(eapPack.data, 0xdd, 4);

	sendData(&eapPack);
	return 0;
}
