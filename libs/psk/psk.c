#include "psk.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include <time.h>

#include "auth.h"
#include "lower.h"
#include "aes128.h"

uint8_t PSK[16] = {0xdd};
uint8_t AK[16] = {0};
uint8_t KDK[16] = {0};

uint8_t identifier = 0;

void keysSettings()
{
	int w[44] = {0};
	keyExpansion(PSK, w);
	uint8_t c0[16] = {0};
	uint8_t buf[16] = {0};
	encript_block(c0, buf, w);
	uint8_t c1[16] = {0};
	c1[15] = 0x01;
	for (int i = 0; i < 16; i++)
		c1[i] = c1[i] ^ buf[i];
	encript_block(c1, AK, w);
	uint8_t c2[16] = {0};
	c2[15] = 0x02;
	for(int i = 0; i < 16; i++)
		c2[i] = c2[i] ^ buf[i];
	encript_block(c2, KDK, w);
}

int startAuth()
{
	keysSettings();
	srand(time(NULL));
	identifier = rand() % 0x100;
	uint8_t rand_s[16];
	for(int i = 0; i < 16; rand_s[i++] = rand() % 0x100);
	printf("RAND_S: ");
	for(int i = 0; i < 16; printf("%02x", rand_s[i++]));
	printf("\n");
	uint8_t flags = 0x00;
	uint8_t id_s = 0x11;
	eap_request eapPack;
	eapPack.code = EAP_REQUEST;
	eapPack.identifier = identifier++;
	eapPack.length = 23;
	eapPack.type = 0xff;
	eapPack.type_data = malloc(18);
	eapPack.type_data[0] = flags;
	memcpy(&eapPack.type_data[1], &rand_s[0], 16);
	eapPack.type_data[17] = id_s;

	printf("FIRST REQUEST:\n");
	printf("Request code = %02x\n", eapPack.code);
	printf("Request identifier = %02x\n", eapPack.identifier);
	printf("Request length = %04x\n", eapPack.length);
	printf("Request type = %02x\n", eapPack.type);
	printf("Reqyest type data: ");
	for(int i = 0; i < eapPack.length - 5; printf("%02x", eapPack.type_data[i++]));
	printf("\nEND\n");


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

void waitAuth()
{
	keysSettings();
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
