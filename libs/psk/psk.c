#include "psk.h"

#include <stdio.h>

#include "auth.h"
#include "lower.h"


int startAuth()
{
}

void waitAuth()
{
	startListener(9090);
	eap_request *pack = getEapRequest();
	printf("EAP package code = %02x\n", pack->code);
	printf("EAP package identifier = %02x\n", pack->identifier);
	printf("EAP package length = %04x\n", pack->length);
	printf("EAP package type = %02x\n", pack->type);
	for(int i = 0; i < pack->length - 5; printf("%02x", pack->type_data[i++]));
	printf("\n");
	sendEapRequest(pack);
}
