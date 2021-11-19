#ifndef SERVER_HPP_
#define SERVER_HPP_

#include <ctime>
#if defined(_WIN32)
#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0600
#endif

// Windows headers
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")

#else
// Linux headers
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <unistd.h>

#endif /*defined(_WIN32)*/
#if defined(_WIN32)
#define GetSocketError() (WSAGetLastError())
#define IsValidSocket(s) ((s) != INVALID_SOCKET)
#define CloseSocket(s) closesocket(s)

#else
#define GetSocketError() (errno)
#define IsValidSocket(s) ((s) >= 0)
#define CloseSocket(s) close(s)
#define SOCKET int

#endif /*defined(_WIN32)*/
#define TIMEOUT 5.0
SOCKET startServer(char *host, char*ip);
const char *GetErrorMsg();
fd_set waitOnClient(SOCKET cliSocket);
const char *getResponse(SOCKET cliSocket);
int SendData(SOCKET socket, char *data);
#endif /*SERVER_HPP_*/
