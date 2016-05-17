#include "aes128.h"
#include <stdint.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>

uint8_t const_Zero[16] = {0};

uint8_t k1[16] = {0};
uint8_t k2[16] = {0};

int w[44] = {0};

void getSubKeys(uint8_t key[16])
{
	uint8_t L[16];
	encript_block(const_Zero, L, w);
	k1[15] = L[15] << 1;
	for(int i = 14; i >= 0; --i)
	{
		k1[i] = (L[i] << 1) | (L[i + 1] >> 7);
	}
	if((L[0] >> 7) != 0)
	{
		k1[15] = k1[15] ^ 0x87;
	}
	k2[15] = k1[15] << 1;
	for(int i = 14; i >= 0; --i)
	{
		k2[i] = (k1[i] << 1) | (k1[i + 1] >> 7);	
	}
	if((k1[0] >> 7) != 0)
	{
		k2[15] = k2[15] ^ 0x87;
	}
}


void xorArrays(uint8_t res[16], uint8_t arr[16])
{
	uint8_t i;
	for (i = 0; i < 16; i++)
		res[i] ^= arr[i];
}

void getCMAC(uint8_t* data, int len, uint8_t res[16])
{
	uint8_t X[16] = {0};
	uint8_t Y[16] = {0};
	uint8_t tmp[16] = {0};
	int n = ceil((double)len/16);

	for(int i = 0; i < n-1; ++i)
	{
		memcpy(&Y[0], &X[0], 16);
		memcpy(&tmp[0], &data[16*i], 16);
		xorArrays(Y, tmp);
		encript_block(Y, X, w);
	}
	
	if(len > 0 && len % 16 == 0)
	{
		memcpy(&Y[0], &data[len - 16], 16);
		xorArrays(Y, k1);
	}
	else
	{
		memset(&Y[0], 0x00, 16);
		uint8_t r = len % 16;
		memcpy(&Y[0], &data[len - r], r);
		Y[r] = 0x80;
		xorArrays(Y, k2);
	}
	xorArrays(Y, X);
	encript_block(Y, res, w);	
}

void initCMAC(uint8_t key[16])
{
	keyExpansion(key, w);
	getSubKeys(key);
}
