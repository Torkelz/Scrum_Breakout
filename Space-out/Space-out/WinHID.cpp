#include "WinHID.h"

HID::HID(){}

HID::HID( HWND p_hwnd )
{
	m_keyboard		= Keyboard( p_hwnd );
	m_mouse			= Mouse( p_hwnd );
	m_observable	= HIDObservable();
}

HID::~HID()
{
	m_mouse.~Mouse();
	m_mouse = NULL;
	m_keyboard.~Keyboard();
	m_keyboard = NULL;
}

void HID::update( UINT message, LPARAM lParam )
{
	UINT dwSize;
	RAWINPUT* pRaw;

	switch (message) 
	{
		case WM_INPUT:
			dwSize = 40;
			static BYTE lpb[40];
    
			GetRawInputData( (HRAWINPUT)lParam, RID_INPUT, 
							lpb, &dwSize, sizeof(RAWINPUTHEADER) );
    
			pRaw = (RAWINPUT*)lpb;
    
			if (pRaw->header.dwType == RIM_TYPEMOUSE) 
			{
				if ( m_mouse.click(pRaw, RI_MOUSE_LEFT_BUTTON_DOWN) )
				{
					POINT t_point = m_mouse.getPosition();
					vec2 t_Vpoint;
					t_Vpoint.x = (float)t_point.x;
					t_Vpoint.y = (float)t_point.y;
					m_observable.broadcastLeftClick( t_Vpoint );
				}

				if( m_mouse.click(pRaw, RI_MOUSE_RIGHT_BUTTON_DOWN) )
				{
					POINT t_point = m_mouse.getPosition();
					vec2 t_Vpoint;
					t_Vpoint.x = (float)t_point.x;
					t_Vpoint.y = (float)t_point.y;
					m_observable.broadcastRightClick(t_Vpoint);
				}

				POINT t_point = m_mouse.getPosition();
				vec2 t_Vpoint;
				t_Vpoint.x = (float)t_point.x;
				t_Vpoint.y = (float)t_point.y;
				m_observable.broadcastMousePos(t_Vpoint);
			}

			if (pRaw->header.dwType == RIM_TYPEKEYBOARD)
			{
				if(pRaw->data.keyboard.Flags == 0)
					m_observable.broadcastKeyPress( m_keyboard.keyDown(pRaw) );
			}
			break;
		default:
			
			break;
	}
}

HIDObservable* HID::getObservable()
{
	return &m_observable;
}