#ifndef __AUTH_EAP_H__
#define __AUTH_EAP_H__

#include "eap.h"

typedef struct
{
	uint8_t code;
	uint8_t identifier;
	uint16_t length;
	uint8_t type;
	uint8_t *type_data;
}eap_request;

eap_request* eapPackToReq(const eap_package* const pack);
eap_package* eapReqToPack(const eap_request* const req);
eap_request* auth_round(const eap_request* const req);
eap_request* getEapRequest();
int sendEapRequest(const eap_request* const req);

#endif//__AUTH_EAP_H__
