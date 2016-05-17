#include "aes128.h"
#include <stdint.h>
#include <stdio.h>

uint8_t const_Zero[16] = {0};

uint8_t k1[16] = {0};
uint8_t k2[16] = {0};

int w[44] = {0};

uint8_t K[16] = {0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6, 0xab, 0xf7, 0x15, 0x88, 0x09, 0xcf, 0x4f, 0x3c};

void getSubKeys(uint8_t key[16])
{
	uint8_t L[16];
	encript_block(const_Zero, L, w);
	printf("ENCRIPTED ZERO: ");
	for(int i = 0; i < 16; printf("%02x", L[i++]));
	printf("\n");
	k1[15] = L[15] << 1;
	for(int i = 14; i >= 0; --i)
	{
		k1[i] = (L[i] << 1) | (L[i + 1] >> 7);
	}
	if((L[0] >> 7) != 0)
	{
		k1[15] = k1[15] ^ 0x87;
	}
	printf("SubKey1: ");
	for(int i = 0; i < 16; printf("%02x", k1[i++]));
	printf("\n");
	k2[15] = k1[15] << 1;
	for(int i = 14; i >= 0; --i)
	{
		k2[i] = (k1[i] << 1) | (k1[i + 1] >> 7);	
	}
	if((k1[0] >> 7) != 0)
	{
		k2[15] = k2[15] ^ 0x87;
	}
	printf("SubKey2: ");
	for(int i = 0; i < 16; printf("%02x", k2[i++]));
	printf("\n");
}

void getCMAC(uint8_t* data, int len, uint8_t res[16])
{
	int n = len / 16;
	uint8_t flag = 1;
	if(len % 16 != 0) ++n;
	if(n == 0)
	{
		n = 1;
		flag = 0;
	}
	else
	{
		if (len % 16 == 0)
		{
			flag = 1;
		}
		else
		{
			flag = 0;
		}
	}
	if(flag == 1)
	{
		//TODO:
	}
	else
	{
	}
}

int main()
{
	keyExpansion(K, w);
	getSubKeys(K);
 	uint8_t M[16] = {0x00};
	getCMAC(NULL, 0, M);
	printf("CHECK: ");
	for(int i = 0; i < 16; printf("%02x", M[i++]));
	printf("\n");
	return 0;
}
