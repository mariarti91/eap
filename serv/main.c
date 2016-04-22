#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "auth.h"

#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <sys/time.h>

int main()
{
	printf("I will became a EAP server\n");
	eap_request eapPack;
	eapPack.code = EAP_REQUEST;
	eapPack.identifier = 0xee;
	eapPack.length = 8;
	eapPack.type = 0xcc;
	eapPack.type_data = malloc(3);
	memset(eapPack.type_data, 0xdd, 3);

	eap_request *res = auth_round(&eapPack);

	printf("Response code = %02x\n", res->code);
	printf("Response identifier = %02x\n", res->identifier);
	printf("Response package length = %04x\n", res->length);
	printf("Response type = %02x\n", res->type);
	printf("Response payload: ");
	for(int i = 0; i < res->length - 5; printf("%02x", res->type_data[i++]));
	printf("\n");
	
	return 0;
}
