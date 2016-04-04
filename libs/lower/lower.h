#ifndef __LOWER_H__
#define __LOWER_H__

#include <sys/socket.h>
#include <unistd.h>
#include <sys/time.h>
#include <netinet/in.h>

int initLower();

void sendData(const uint8_t* const data, const int size);

int getData(uint8_t* data);

#endif//__LOWER_H__
