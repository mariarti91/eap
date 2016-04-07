#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "eap.h"

#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <sys/time.h>

int main()
{
	printf("I will became a EAP server\n");
	eap_package eapPack;
	eapPack.code = EAP_REQUEST;
	eapPack.identifier = 0xee;
	eapPack.length = 8;
	eapPack.data = malloc(4);
	memset(eapPack.data, 0xdd, 4);

	eap_package* response = eapExchange(&eapPack);

	printf("Response code = %02x\n", response->code);
	printf("Response identifier = %02x\n", response->identifier);
	printf("Response package length = %04x\n", response->length);
	printf("Response payload: ");
	for(int i = 0; i < response->length - 4; printf("%02x", response->data[i++]));
	printf("\n");
	
	free(response);

	return 0;
}
