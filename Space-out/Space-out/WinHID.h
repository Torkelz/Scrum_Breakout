#ifndef HID_H
#define HID_H

#include <Windows.h>

#include "WinKeyboard.h"
#include "WinMouse.h"
#include "HIDObservable.h"

class HID
{
public:
	HID();
	HID( HWND p_hwnd );
	~HID();

	void update( UINT message, LPARAM lParam );
	HIDObservable*	getObservable();
private:
	HIDObservable	m_observable;
	Mouse			m_mouse;
	Keyboard		m_keyboard;
};

#endif

// http://www.toymaker.info/Games/html/raw_input.html