#include "Header/LinHid.h"

HID::HID(GLFWwindow* p_pHwnd)
{
	m_pHwnd = p_pHwnd;

	m_pObservable	= new HIDObservable();
	m_pMouse		= new Mouse(p_pHwnd);
	m_pKeyboard 	= new Keyboard(p_pHwnd);
}

HID::~HID()
{
	m_pObservable->~HIDObservable();
	m_pObservable = NULL;

	m_pMouse->~Mouse();
	m_pMouse = NULL;
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

void HID::update(int p_action, int p_key)
{
//	switch(p_action)
//	{
//	case GLFW_PRESS:
//			m_pObservable->broadcastKeyPress( m_pKeyboard->KeyDown(p_key) );
//		break;
//	}
	if(SDL_PollEvent(&event))
		{
			if(event.type == SDL_QUIT)
			{

			}

			if(event.type == SDL_KEYDOWN)
			{
				SDLKey keyPressed = event.key.keysym.sym;

					switch (keyPressed)
					{
					   case SDLK_ESCAPE:
						 // gameRunning = false;
						   glfwTerminate();
						   int i;
						   i = 0;
						  break;
					}
			}
		}
}

HIDObservable* HID::getObservable()
{
	return m_pObservable;
}
