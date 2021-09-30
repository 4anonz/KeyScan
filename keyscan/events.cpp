#include "events.hpp"

//	This function is used for sending a single keyboard event.
//	It return 0 on success and -1 on error
int Event::kbtEventSimulate(SHORT vKey) {
		INPUT inputs[2] {{}};						//Input array structure 
		ZeroMemory(inputs, sizeof(inputs));
		
		inputs[0].type = INPUT_KEYBOARD;			//input type
		inputs[0].ki.wVk = vKey;
		
		inputs[1].type = INPUT_KEYBOARD;
		inputs[1].ki.wVk = vKey;
		inputs[1].ki.dwFlags = KEYEVENTF_KEYUP;		//the key is been realesed
		
		UINT uSent = SendInput(ARRAYSIZE(inputs), inputs, sizeof(INPUT));
		if(uSent != ARRAYSIZE(inputs))
			return -1;								// An error
		
		return 0;
}


//	This function is use for sending keyboard commands like CLT+C, ALT+F4. etc.
//	It returns 0 on success and -1 on error(s).
int Event::kbtEventSimulateMul(struct MULKEY vKeys[MAXKEY], int len, char ch) {
		
		int in;
		if(len == 1 && !ch)
			in = 2;
		else if(len == 1 && ch)
			in = 4;
		else if(len > 1 && ch)
			in = len * len + 2; 
		else
			in = len * len;
		INPUT inputs[in] {{}};
		int i {0}, tmp {0}, p {0};
		/*int p { 0 };
		if(ch && in > 2)
			p = in - 2;
		else
			p = in;
		*/
		// First get each key and press it down
		for(; i < len; i++) {
				inputs[i].type = INPUT_KEYBOARD;		
				inputs[i].ki.wVk = vKeys[i].vKey;
				tmp += 1;
		}
		
		HKL kbl = GetKeyboardLayout(0);
		// Get virtual key code of the character
		SHORT vKeyCode = VkKeyScanEx(ch, kbl);
		if(ch) {
			inputs[i].type = INPUT_KEYBOARD;
			inputs[i].ki.wVk = vKeyCode;
			i += 1;
			inputs[i].type = INPUT_KEYBOARD;
			inputs[i].ki.wVk = vKeyCode;
			inputs[i].ki.dwFlags = KEYEVENTF_KEYUP;
		}
		if(ch) i += 1;
		tmp -= 1;
		// Then press each of keys up in reserve order in which they were pressed
		for(; p < len; i++) {
				inputs[i].type = INPUT_KEYBOARD;
				inputs[i].ki.wVk = inputs[tmp].ki.wVk;
				inputs[i].ki.dwFlags = KEYEVENTF_KEYUP;
				p++;
				tmp -= 1;
		}
		
		UINT uSent = SendInput(ARRAYSIZE(inputs), inputs, sizeof(INPUT));
		if(uSent != ARRAYSIZE(inputs))
			return -1;
		
		return 0;
}



// This function is use for moving the mouse pointer to a certain position
int Event::mouseEventSimulateMov(LONG x, LONG y) {
		INPUT mouse[1] {{}};
		ZeroMemory(mouse, sizeof(mouse));
		
		mouse[0].type = INPUT_MOUSE;			//Mouse input
		mouse[0].mi.dx = x;						//X coordinate 0 - 65535
		mouse[0].mi.dy = y;						//Y coordinate 0 - 65535
		mouse[0].mi.mouseData = 0;
		mouse[0].mi.dwFlags = MOUSEEVENTF_MOVE | MOUSEEVENTF_ABSOLUTE;
		mouse[0].mi.time = 0;
		mouse[0].mi.dwExtraInfo = 0;
		
		UINT uSent = SendInput(ARRAYSIZE(mouse), mouse, sizeof(INPUT));
		if(uSent != ARRAYSIZE(mouse))
			return -1;
		
		return 0;
}


//	This function is use for left, right or middle clicking..
int Event::mouseEventSimulateClick(int rol) {
		int fFlag, sFlag;
		if(rol == 0) {
			fFlag = MOUSEEVENTF_LEFTDOWN;
			sFlag = MOUSEEVENTF_LEFTUP;
		}else if(rol == 1) {
			fFlag = MOUSEEVENTF_RIGHTDOWN;
			sFlag = MOUSEEVENTF_RIGHTUP;
		} else {
			fFlag = MOUSEEVENTF_MIDDLEDOWN;
			sFlag = MOUSEEVENTF_MIDDLEUP;
		}
		
		
		INPUT click[2] {{}};
		ZeroMemory(click, sizeof(click));
		
		//Fill in the structure
		//	For mouse button been pressed
		click[0].type = INPUT_MOUSE;			//Mouse event
		click[0].mi.dwFlags = fFlag;			//Virtual Key code
		
		//	For mouse botton been released
		click[1].type = INPUT_MOUSE;
		click[1].mi.dwFlags = sFlag;			//Key up
		
		UINT uSent = SendInput(ARRAYSIZE(click), click, sizeof(INPUT));
		if(uSent != ARRAYSIZE(click))
			return -1;
		
		return 0;
}


int Event::GetKeyCode(char *k) {
	
	if(strcmp(k, "CTRL") == 0)
		return CTRL;
	else if(strcmp(k, "ENTER") == 0)
		return ENTER;
	else if(strcmp(k, "SHIFT") == 0)
		return SHIFT;
	else if(strstr(k, "ALT"))
		return ALT;
	else if(strcmp(k, "CAPS") == 0)
		return CAPS;
	else if(strcmp(k, "ESC") == 0)
		return ESC;
	else if(strcmp(k, "PAGEUP") == 0)
		return PUP;
	else if(strcmp(k, "PAGEDOWN") == 0)
		return PDOWN;
	else if(strcmp(k, "HOME") == 0)
		return HOME;
	else if(strcmp(k, "LEFTARROW") == 0)
		return LEFT;
	else if(strcmp(k, "RIGHTARROW") == 0)
		return RIGHT;
	else if(strcmp(k, "UPARROW") == 0)
		return UP;
	else if(strcmp(k, "DOWNARROW") == 0)
		return DOWN;
	else if(strcmp(k, "PRINTSCRN") == 0)
		return PRINTSCRN;
	else if(strcmp(k, "INSERT") == 0)
		return INSERT;
	else if(strcmp(k, "DELETE") == 0)
		return DEL_KEY;
	else if(strcmp(k, "WIN") == 0)
		return WIN;
	else if(strcmp(k, "F1") == 0)
		return VK_F1;
	else if(strcmp(k, "F2") == 0)
		return VK_F2;
	else if(strcmp(k, "F3") == 0)
		return VK_F3;
	else if(strstr(k, "F4"))
		return VK_F4;
	else if(strcmp(k, "F5") == 0)
		return VK_F5;
	else if(strcmp(k, "F6") == 0)
		return VK_F6;
	else if(strcmp(k, "F7") == 0)
		return VK_F7;
	else if(strcmp(k, "F8") == 0)
		return VK_F8;
	else if(strcmp(k, "F9") == 0)
		return VK_F9;
	else if(strcmp(k, "F10") == 0)
		return VK_F10;
	else if(strcmp(k, "F11") == 0)
		return VK_F11;
	else if(strcmp(k, "F12") == 0)
		return VK_F12;
		
	return -1;
}
