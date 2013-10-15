#include "Header/LinKeyboard.h"

Keyboard::Keyboard(GLFWwindow* p_pHwnd)
{
	m_pHwnd = p_pHwnd;
}

Keyboard::~Keyboard()
{
	m_pHwnd = NULL;
}

unsigned short Keyboard::KeyDown(unsigned short p_key)
{
	unsigned short ushort;
//http://www.grandmaster.nu/blog/?page_id=609

	switch(p_key)
	{
	case GLFW_KEY_W:
		ushort = 0x41;
		break;
	case GLFW_KEY_A:
		ushort = 0x44;
		break;
	case GLFW_KEY_S:
		ushort = 0x57;
		break;
	case GLFW_KEY_D:
		ushort = 0x53;
		break;
	case GLFW_KEY_ESCAPE:
		ushort = 0x1B;
		break;
	case GLFW_KEY_LEFT_CONTROL:

		break;

	case GLFW_KEY_UNKNOWN:
		ushort = 0x00;
		break;
	}

	return ushort;
}
