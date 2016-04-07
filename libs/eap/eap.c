#include "eap.h"

#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>

#include "lower.h"

#include <stdio.h>

int eapToData(const eap_package* pack, uint8_t* data)
{
	uint16_t size = pack->length;
	data = realloc(data, size);
	memcpy(data, pack, 2);
	size = htons(size);
	memcpy(&data[2], &size, 2);
	size = ntohs(size);
	memcpy(&data[4], pack->data, size - 4);
	return size;
}

int dataToEap(const uint8_t* data, const int size, eap_package* pack)
{
	if(size < 4) return -2;
	uint16_t length;
	memcpy(&length, &data[2], 2);
	length = ntohs(length);
	if(length > size) return -1;
	pack = realloc(pack, sizeof(eap_package));
	memcpy(pack, data, 2);
	pack->length = length;
	pack->data = malloc(length - 4);
	memcpy(pack->data, &data[4], length - 4);
	return 0;
}

eap_package* eapExchange(const eap_package* const pack)
{
	uint8_t exchange_id = pack->identifier;
	initLower();
	uint8_t *requ_data = malloc(0);
	int request_size = eapToData(pack, requ_data);

	uint8_t *resp_data = malloc(0);
	eap_package *response = malloc(0);
	
	do
	{
		sendData(requ_data, request_size);
		int response_size = getData(resp_data);
		if(response_size < 0) continue;

		if(dataToEap(resp_data, response_size, response) < 0) continue;

		if(response->identifier != exchange_id) continue;

		break;
	}
	while(1);

	free(requ_data);
	return response;
}

eap_package* getEapPackage()
{
	waitingForConnection();
	uint8_t *req_data = malloc(0);
	eap_package *request = malloc(0);

	do
	{
		int req_size = getData(req_data);

		if(req_size < 0) continue;

		if(dataToEap(req_data, req_size, request) < 0 ) continue;

		break;
	}
	while(1);
	free(req_data);
	return request;
}

int sendEapPackage(eap_package* pack)
{
	uint8_t *data = malloc(0);
	int data_size = eapToData(pack, data);
	sendData(data, data_size);
	closeConnection();
	return 0;
}
