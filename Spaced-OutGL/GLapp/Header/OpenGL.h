#ifndef OPENGL_H_
#define OPENGL_H_

#include "GLApp.h"
#include <stdio.h>
#include <stdlib.h>

using namespace glm;

class OpenGL : public GLApp{
	public:
	OpenGL();
	~OpenGL();

	void 			initApp();
	void 			updateScene(float p_dt);
	void 			drawScene();
	static void 	messageCallback(GLFWwindow* p_pMainWnd, int p_key, int p_scanCode, int p_action, int p_mods);

private:
	mat4x4 			m_camView;
	float			rotation;
};

#endif /* OPENGL_H_ */
