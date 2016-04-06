#include <stdio.h>
#include <stdlib.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <sys/time.h>
#include <stdint.h>

#include "eap.h"
#include "lower.h"

int main()
{
	printf("I will became a EAP peer\n");

	startListener(9090);
	eap_package *pack = getEapPackage();
	printf("EAP package code = %02x\n", pack->code);
	printf("EAP package identifier = %02x\n", pack->identifier);
	printf("EAP package length = %04x\n", pack->length);
	for(int i = 0; i < pack->length - 4; printf("%02x", pack->data[i++]));
	printf("\n");
	return 0;
}
