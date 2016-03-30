#include "eap.h"

#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>

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
