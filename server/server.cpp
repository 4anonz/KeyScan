#include "server.hpp"
#include <string.h>
#include <iostream>



SOCKET startServer(char *host, char *port) {
	
	
	struct addrinfo hints, *bindAddr;
	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET;			// IPv4
	hints.ai_socktype = SOCK_STREAM;	// TCP socket
	hints.ai_flags = AI_PASSIVE;		// flag
	

	//Configure the local address and port number..
	if(getaddrinfo(host, port, &hints, &bindAddr))
		return -1;
	
	// create a listening socket
	SOCKET srvSocket;
	srvSocket = socket(bindAddr->ai_family, bindAddr->ai_socktype,
					   bindAddr->ai_protocol);
	if(!srvSocket)
		return -1;
	
	if(bind(srvSocket, bindAddr->ai_addr, bindAddr->ai_addrlen))
		return -1;
	
	if(listen(srvSocket, 10) < 0)
		return -1;
	freeaddrinfo(bindAddr);
	return srvSocket;
}



const char *GetErrorMsg() {
	
#if defined(_WIN32)
	static char message[256] { 0 };
	FormatMessage(
	FORMAT_MESSAGE_FROM_SYSTEM|FORMAT_MESSAGE_IGNORE_INSERTS,
	0, WSAGetLastError(), 0, message, 256, 0);
	char *nl = strrchr(message, '\n');
	if (nl) *nl = 0;
	return message;
#else
	return strerror(errno);
#endif
}


fd_set waitOnClient(SOCKET cliSocket) {
	fd_set reads;
	FD_ZERO(&reads);
	FD_SET(cliSocket, &reads);
	/*
	struct timeval timeout;
	timeout.tv_sec = 0;
	timeout.tv_usec = 200000;*/
	
	select(cliSocket+1, &reads, 0, 0, 0);

	return reads;
}


const char *getResponse(SOCKET cliSocket) {
	static char msg[4098];
	memset(msg, 0, sizeof(msg));
	const clock_t startTime = clock();
	while(1) {
		if((clock() - startTime) / CLOCKS_PER_SEC > TIMEOUT)
			break;
		fd_set reads;
		reads = waitOnClient(cliSocket);
		if(FD_ISSET(cliSocket, &reads)) {
			recv(cliSocket, msg, sizeof(msg), 0);
			break;
		}
	}
	return msg;
}


int SendData(SOCKET socket, char *data) {
	int bytesSent = send(socket, data, strlen(data), 0);
	if (bytesSent < 1)
		return -1;
	return bytesSent;
}
