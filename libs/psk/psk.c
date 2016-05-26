#include "psk.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include <time.h>

#include "auth.h"
#include "lower.h"
#include "aes128.h"
#include "aescmac.h"

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

	printf("SECOND MESSAGE:\n");
	printf("Response code = %02x\n", res->code);
	printf("Response identifier = %02x\n", res->identifier);
	printf("Response package length = %04x\n", res->length);
	printf("Response type = %02x\n", res->type);
	printf("Response payload: ");
	for(int i = 0; i < res->length - 5; printf("%02x", res->type_data[i++]));
	printf("\n");

	uint8_t rand_p[16];
	memcpy(&rand_p[0], &res->type_data[17], 16);
	printf("GETTED RAND_P: ");
	for(int i = 0; i < 16; printf("%02x", rand_p[i++]));
	printf("\n");
	uint8_t id_p = res->type_data[res->length - 6];
	printf("GETTED ID_P: %02x\n", id_p);
	
	initCMAC(AK);
	uint8_t getted_mac_p[16];
	memcpy(&getted_mac_p[0], &res->type_data[33], 16);
	printf("GETTED MAC P: ");
	for(int i = 0; i < 16; printf("%02x", getted_mac_p[i++]));
	printf("\n");
	uint8_t *mac_data = malloc(34);
	mac_data[0] = id_p;
	mac_data[1] = id_s;
	memcpy(&mac_data[2], &rand_s[0], 16);
	memcpy(&mac_data[18], &rand_p[0], 16);
	uint8_t calc_mac_p[16];
	getCMAC(mac_data, 34, calc_mac_p);
	free(mac_data);
	printf("CALC MAC P: ");
	for(int i = 0; i < 16; printf("%02x", calc_mac_p[i++]));
	printf("\n");

	return 0;
}

void waitAuth()
{
	keysSettings();
	startListener(9090);
	printf("Fire EAP message\n");
	eap_request *pack = getEapRequest();
	printf("EAP package code = %02x\n", pack->code);
	printf("EAP package identifier = %02x\n", pack->identifier);
	printf("EAP package length = %04x\n", pack->length);
	printf("EAP package type = %02x\n", pack->type);
	for(int i = 0; i < pack->length - 5; printf("%02x", pack->type_data[i++]));
	printf("\n");
	uint8_t rand_s[16];
	memcpy(&rand_s[0], &pack->type_data[1], 16);
	uint8_t id_s = pack->type_data[17];
	uint8_t id_p = 0x22;
	uint8_t rand_p[16];
	srand(time(NULL));
	for(int i = 0; i < 16; rand_p[i++] = rand() % 0x100);
	printf("RAND_P: ");
	for(int i = 0; i < 16; printf("%02x", rand_p[i++]));
	printf("\n");
	uint8_t flags = 0x40;
	eap_request res;
	res.code = 2;
	res.identifier = pack->identifier;
	res.type = pack->type;
	res.type_data = malloc(50);
	res.type_data[0] = flags;
	memcpy(&res.type_data[1], &rand_s[0], 16);
	memcpy(&res.type_data[17], &rand_p[0], 16);
	initCMAC(AK);
	uint8_t mac_p[16];
	uint8_t *mac_data = malloc(34);
	mac_data[0] = id_p;
	mac_data[1] = id_s;
	memcpy(&mac_data[2], &rand_s[0], 16);
	memcpy(&mac_data[18], &rand_p[0], 16);
	getCMAC(mac_data, 34, mac_p);
	free(mac_data);
	memcpy(&res.type_data[33], &mac_p[0], 16);
	res.type_data[49] = id_p;
	res.length = 55;
	printf("Second EAP message:\n");
	printf("EAP package code = %02x\n", res.code);
	printf("EAP package identifier = %02x\n", res.identifier);
	printf("EAP package length = %04x\n", res.length);
	printf("EAP package type = %02x\n", res.type);
	for(int i = 0; i < res.length - 5; printf("%02x", res.type_data[i++]));
	printf("\n");
	sendEapRequest(&res);	
}
