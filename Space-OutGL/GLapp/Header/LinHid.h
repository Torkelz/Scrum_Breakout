#ifndef HID_H_
#define HID_H_

#include <glm/glm.hpp>
#include "GLUtil.h"

using namespace glm;

class HID
{
private:
	GLFWwindow* m_pHwnd;
public:
	HID(GLFWwindow* p_pHwnd);
	~HID();

	void update(int p_action);
};
// http://www.glfw.org/docs/3.0/group__keys.html

#endif /* LINHID_H_ */
