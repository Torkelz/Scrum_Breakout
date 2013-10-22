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

	switch(p_key)
	{
	case GLFW_KEY_A:
		ushort = 0x41;
		break;
	case GLFW_KEY_D:
		ushort = 0x44;
		break;
	case GLFW_KEY_W:
		ushort = 0x57;
		break;
	case GLFW_KEY_S:
		ushort = 0x53;
		break;
	case GLFW_KEY_Q:
		ushort = 0x51;
		break;
	case GLFW_KEY_E:
		ushort = 0x45;
		break;
	case GLFW_KEY_SPACE:
		ushort = 0x20;
		break;
	case GLFW_KEY_LEFT:
		ushort = 2190;
		break;
	case GLFW_KEY_RIGHT:
		ushort = 2192;
		break;
	case GLFW_KEY_ESCAPE:
		ushort = 0x1b;
		break;

	case GLFW_KEY_UNKNOWN:
		ushort = 0x00;
		break;
	}

	return ushort;
}
