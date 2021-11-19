#ifndef CONNECT_HPP_
#define CONNECT_HPP

#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0600
#endif

#include <winsock2.h>
#include <ws2tcpip.h>
#include <cstring>

#pragma comment(lib, "ws2_32.lib")

SOCKET ConnectServer(char *host, char *port);
bool SendData(SOCKET fd, char *data);
void SendSuccess(int fd);
void SendUnsuccess(int fd);

#endif /*CONNECT_HPP_*/
