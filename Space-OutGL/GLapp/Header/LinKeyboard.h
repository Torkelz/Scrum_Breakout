#ifndef KEYBOARD_H_
#define KEYBOARD_H_

#include "GLUtil.h"

class Keyboard
{
private:
	GLFWwindow* m_pHwnd;
public:
	Keyboard(GLFWwindow* p_pHwnd);
	~Keyboard();

	unsigned short KeyDown( unsigned short p_key );
};

// http://www.glfw.org/docs/3.0/group__keys.html

#endif /* LINKEYBOARD_H_ */