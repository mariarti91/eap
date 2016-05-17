#ifndef __AES_128_H__
#define __AES_128_H__

#define Nb 4
#define Nk 4
#define Nr 10

#include <stdint.h>

int keyExpansion(uint8_t key[4 * Nk], int w[Nb * (Nr + 1)]);

int encript_block(uint8_t input[4 * Nb], uint8_t output[4 * Nb], int w[Nb * (Nr + 1)]);

int decript_block(uint8_t input[4 * Nb], uint8_t output[4 * Nb], int w[Nb * (Nr + 1)]);

#endif //__AES_128_H__
