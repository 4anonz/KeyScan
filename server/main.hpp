#include <cstdlib>
#include <cstring>
#include <iostream>
#include <fstream>
#include "server.cpp"

void ExecuteCommand(char *cmd);
void printDefault();
void printHelp();

static bool isServerStarted { false }, isSession { false };
static char lhost[100] { "" };
static char lport[20] { "4444" };
static SOCKET srvSocket;
static SOCKET cliSocket;
static char cliIP[100] { "" }, cliSrv[100];
static int bytesSent {0};
int editFile(std::string lhost, std::string lport);

const char *red = "\033[1;31m",
*gre = "\033[1;32m",
*blu = "\033[1;34m",
*res = "\e[0m";
