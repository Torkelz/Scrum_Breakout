#include "Header/LinHid.h"

HID::HID(GLFWwindow* p_pHwnd)
{
	m_pHwnd = p_pHwnd;

	m_pObservable	= new HIDObservable();
	m_pKeyboard 	= new Keyboard(p_pHwnd);
}

HID::~HID()
{
	m_pObservable->~HIDObservable();
	m_pObservable = NULL;

	m_pKeyboard->~Keyboard();
	m_pKeyboard = NULL;
	SDL_Quit();

}

bool HID::initDevices()
{
	SDL_Init(SDL_INIT_EVERYTHING);
	SDL_SetVideoMode(640, 480, 32, SDL_NOFRAME);
	return true;
}

void HID::update(int p_key)
{
	if(p_key != 0x00)
		m_pObservable->broadcastKeyPress( m_pKeyboard->KeyDown(p_key) );
}

HIDObservable* HID::getObservable()
{
	return m_pObservable;
}
