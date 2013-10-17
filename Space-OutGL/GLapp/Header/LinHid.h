#ifndef HID_H_
#define HID_H_

#include <glm/glm.hpp>
#include "GLUtil.h"
#include "HIDObservable.h"
#include "LinKeyboard.h"
#include <SDL/SDL.h>

#define KEY_RELEASE 0
#define KEY_PRESS 1

using namespace glm;

class HID
{
private:
	GLFWwindow* m_pHwnd;

	HIDObservable*	m_pObservable;
	Keyboard*		m_pKeyboard;

public:
	HID(GLFWwindow* p_pHwnd);
	~HID();
	SDL_Event event;
	bool initDevices();

	void update(int p_key);

	HIDObservable*	getObservable();
};

#endif /* LINHID_H_ */
