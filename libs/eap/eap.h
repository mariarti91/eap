#ifndef __EAP_H__
#define __EAP_H__

#include <stdint.h>

typedef enum
{
	EAP_REQUEST = 1,
	EAP_RESPONSE,
	EAP_SUCCESS,
	EAP_FAILURE,
}eap_code_fild;

typedef struct
{
	uint8_t code;
	uint8_t identifier;
	uint16_t length;
	uint8_t *data;
}eap_package;

int eapToData(const eap_package* pack, uint8_t* data);

#endif//__EAP_H__
