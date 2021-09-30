#include "connect.hpp"

SOCKET ConnectServer(char *host, char *port) {
	
	struct addrinfo hints;
	memset(&hints, 0, sizeof(hints));
	/**
	 * Fill in the addrinfo structure.. for now just set the socket type
	 * to TCP socket, we'll use the server's address family. i.e wether
	 * IPv4 or IPv6 and so also the protocol
	 * */
	hints.ai_socktype = SOCK_STREAM;		//TCP socket..
	struct addrinfo *srvAddr;
	if(getaddrinfo(host, port, &hints, &srvAddr))
		return -1;
	
	/*Create the connection socket*/
	SOCKET peerSocket;
	peerSocket = socket(srvAddr->ai_family, srvAddr->ai_socktype,
						srvAddr->ai_protocol);
	if(peerSocket == INVALID_SOCKET)
		return -1;
		
	if(connect(peerSocket, srvAddr->ai_addr, srvAddr->ai_addrlen))
		return -1;
	
	return peerSocket;
}


bool SendData(SOCKET fd, char *data) {
	if(send(fd, data, strlen(data), 0) < 1)
		return false;
	return 0;
}

void SendSuccess(int fd) {
	char msg[10] = "Success!!";
	SendData(fd, msg);
}

void SendUnsuccess(int fd) {
	char msg[14] = "Unsuccess!!";
	SendData(fd, msg);
}
