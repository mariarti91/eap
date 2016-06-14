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
#include <netinet/in.h>

uint8_t PSK[16] = {0xdd};
uint8_t AK[16] = {0};
uint8_t KDK[16] = {0};
uint8_t TEK[16] = {0};

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

void keysDerive(uint8_t rand[16])
{
	printf("Derivatig keys... ");
	int w[44] = {0};
	keyExpansion(KDK, w);
	uint8_t buf[16] = {0};
	encript_block(rand, buf, w);
	uint8_t c[16] = {0};
	c[15] = 0x01;
	for(int i = 0; i < 16; ++i)
		buf[i] ^= c[i];
	encript_block(buf, TEK, w);
	printf("OK\n");
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
	printf("Checking cmak... ");
	for(int i = 0; i < 16; i++)
	{
		if (getted_mac_p[i] != calc_mac_p[i])
		{
			printf("ERROR!\n");
			return -1;
		}
	}
	printf("OK\n");
	keysDerive(rand_p);
	printf("TEK: ");
	for(int i = 0; i < 16; printf("%02x", TEK[i++]));
	printf("\n");

	eapPack.code = EAP_REQUEST;
	eapPack.identifier = identifier++;
	eapPack.length = 59;
	eapPack.type = 0xff;
	eapPack.type_data = malloc(54);
	eapPack.type_data[0] = 0x80;
	memcpy(&eapPack.type_data[1], &rand_s[0], 16);
	uint8_t mac_s[16];
	mac_data = malloc(17);
	mac_data[0] = id_s;
	memcpy(&mac_data[1], &rand_p[0], 16);
	initCMAC(AK);
	getCMAC(mac_data, 17, mac_s);
	free(mac_data);
	memcpy(&eapPack.type_data[17], &mac_s[0], 16);
	uint8_t* PCHANNEL = malloc(21);
	uint8_t nonce[4] = {0};
	memcpy(&PCHANNEL[0], &nonce[0], 4);
	mac_data = malloc(22);
	memcpy(&mac_data[0], &eapPack, 5);
	memcpy(&mac_data[5], &eapPack.type_data[0], 17);
	initCMAC(TEK);
	uint8_t tag[16] = {0};
	getCMAC(mac_data, 22, tag);
	free(mac_data);
	memcpy(&PCHANNEL[4], &tag[0], 16);
	PCHANNEL[20] = 0x80;
	
	memcpy(&eapPack.type_data[33], &PCHANNEL[0], 21);
	printf("THIRD MESSAGE:\n");
	printf("Request code = %02x\n", eapPack.code);
	printf("Request identifier = %02x\n", eapPack.identifier);
	printf("Request length = %04x\n", eapPack.length);
	printf("Request type = %02x\n", eapPack.type);
	printf("Reqyest type data: ");
	for(int i = 0; i < eapPack.length - 5; printf("%02x", eapPack.type_data[i++]));
	printf("\nEND\n");
	free(res);
	res = auth_round(&eapPack);
	printf("Checking auth from peer... ");
	uint8_t indec = ((res->type_data[res->length - 6]) & 0xc0);
	if (indec == 0xc0) printf("OK\n");
	else printf("FAIL!\n");
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
	free(pack);

	printf("Third EAP message:\n");
	pack = getEapRequest();	
	printf("EAP package code = %02x\n", pack->code);
	printf("EAP package identifier = %02x\n", pack->identifier);
	printf("EAP package length = %04x\n", pack->length);
	printf("EAP package type = %02x\n", pack->type);
	for(int i = 0; i < pack->length - 5; printf("%02x", pack->type_data[i++]));
	printf("\n");

	uint8_t getted_mac_s[16];
	memcpy(&getted_mac_s[0], &pack->type_data[17], 16);
	mac_data = malloc(17);
	mac_data[0] = id_s;
	memcpy(&mac_data[1], &rand_p[0], 16);
	initCMAC(AK);
	uint8_t calc_mac_s[16];
	getCMAC(mac_data, 17, calc_mac_s);
	free(mac_data);
	printf("Checking MAC_S... ");
	for(int i = 0; i < 16; ++i)
	{
		if (calc_mac_s[i] != getted_mac_s[i])
		{
			printf("FAIL!\n");
			return;
		}
	}
	printf("OK\n");
	uint8_t *PCHANNEL = malloc(pack->length - 39);
	memcpy(&PCHANNEL[0], &pack->type_data[33], pack->length - 39);
	uint32_t nonce;
	memcpy(&nonce, &PCHANNEL[0], 4);
	nonce = ntohl(nonce);	
	keysDerive(rand_p);
	printf("TEK: ");
	for(int i = 0; i < 16; printf("%02x", TEK[i++]));
	printf("\n");
	uint8_t getted_tag[16];
	memcpy(&getted_tag[0], &PCHANNEL[4], 16);
	mac_data = malloc(22);
	memcpy(&mac_data[0], pack, 5);
	memcpy(&mac_data[5], &pack->type_data[0], 17);
	initCMAC(TEK);
	uint8_t calc_tag[16] = {0};
	getCMAC(mac_data, 22, calc_tag);
	free(mac_data);
	printf("Checking Tag... ");
	for(int i = 0; i < 16; ++i)
	{
		if (getted_tag[i] != calc_tag[i])
		{
			printf("FAIL!\n");
			return;
		}
	}
	printf("OK\n");

	res.code = 2;
	res.identifier = pack->identifier;
	res.length = 42;
	res.type = 0xff;
	res.type_data = malloc(38);
	res.type_data[0] = 0xc0;
	memcpy(&res.type_data[1], &rand_s[0], 16);
	free(PCHANNEL);
	PCHANNEL = malloc(21);
	nonce = htonl(++nonce);
	memcpy(&PCHANNEL[0], &nonce, 4);
	mac_data = malloc(22);
	memcpy(&mac_data[0], &res, 5);
	memcpy(&mac_data[5], &res.type_data[0], 17);
	initCMAC(TEK);
	uint8_t tag[16] = {0};
	getCMAC(mac_data, 22, tag);
	free(mac_data);
	memcpy(&PCHANNEL[4], &tag[0], 16);
	PCHANNEL[20] = 0x80;
	memcpy(&res.type_data[17], &PCHANNEL[0], 21);
	sendEapRequest(&res);
}
