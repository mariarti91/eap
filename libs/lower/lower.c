#include "lower.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_SIZE 1050
uint8_t sock_buffer[BUFFER_SIZE];

int sock = -1;
int listener = -1;

int startListener(const int port)
{
	listener = socket(AF_INET, SOCK_STREAM, 0);
	if(listener < 0)
	{
		printf("Could not create listener\n");
		return -1;
	}

	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	addr.sin_addr.s_addr = htonl(INADDR_ANY);

	if(bind(listener, (struct sockaddr*)&addr, sizeof(addr)) < 0)
	{
		printf("Could not bind socket\n");
		return -2;
	}
	listen(listener, 1);
	return 0;
}

int waitingForConnection()
{
	if(listener < 0)
	{
		printf("Server is not runned\n");
		return -1;
	}

	sock = accept(listener, NULL, NULL);
	if(sock < 0)
	{
		printf("Could not get socket\n");
		return -1;
	}

	struct timeval timeout;	
	timeout.tv_sec = 10;
	timeout.tv_usec = 0;
	if(setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, (const char*)&timeout, sizeof(struct timeval)) < 0)
	{
		printf("Could not set timeout\n");
	}
	return 0;
}

int closeConnection()
{
	if(sock >= 0) close(sock);
	sock = -1;
	return 0;
}

int initLower()
{
	sock = socket(AF_INET, SOCK_STREAM, 0);
	if(sock < 0)
	{
		printf("Could not create socket\n");
		return -1;
	}

	struct timeval timeout;	
	timeout.tv_sec = 10;
	timeout.tv_usec = 0;
	if(setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, (const char*)&timeout, sizeof(struct timeval)) < 0)
	{
		printf("Could not set timeout\n");
	}

	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(9090);
	addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);

	if(connect(sock, (struct sockaddr*)&addr, sizeof(addr)) < 0)
	{
		printf("Could not connect to host\n");
		return -2;
	}

	return 0;
}

void sendData(const uint8_t* const data, const int size)
{
	send(sock, data, size, 0);
}

int getData(uint8_t* data)
{
	int bytes_read = recv(sock, sock_buffer, BUFFER_SIZE, 0);
	if(bytes_read < 0)
	{
		return -1;
	}

	data = realloc(data, bytes_read);
	memcpy(data, &sock_buffer[0], bytes_read);
	return bytes_read;
}
