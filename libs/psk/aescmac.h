#ifndef __AES_CMAC_H__
#define __AES_CMAC_H__

void initCMAC(uint8_t key[16]);

void getCMAC(uint8_t* data, int len, uint8_t res[16]);

#endif//__AES_CMAC_H__
