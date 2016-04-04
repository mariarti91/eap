#include "eap.h"

#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>

#include "lower.h"

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

eap_package* eap_exchange(const eap_package* const pack)
{
	initLower();
	uint8_t *data = malloc(0);
	int size = eapToData(pack, data);
	sendData(data, size);
	return pack;
}
