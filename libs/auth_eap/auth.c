#include "auth.h"
#include <stdlib.h>
#include <string.h>

//////////////////
#include <stdio.h>
//////////////////

eap_request* eapPackToReq(const eap_package* const pack)
{
	eap_request* req = malloc(sizeof(eap_request));
	memcpy(req, pack, 4);
	req->type = pack->data[0];
	req->type_data = malloc(req->length - 5);
	memcpy(req->type_data, &pack->data[1], req->length - 5);
	return req;
}

eap_package* eapReqToPack(const eap_request* const req)
{
	eap_package* pack = malloc(sizeof(eap_package));
	memcpy(pack, req, 4);
	pack->data = malloc(pack->length - 4);
	pack->data[0] = req->type;
	memcpy(&pack->data[1], req->type_data, pack->length - 5);
	return pack;
} 

eap_request* auth_round(const eap_request* const pack)
{
	return pack;
}
