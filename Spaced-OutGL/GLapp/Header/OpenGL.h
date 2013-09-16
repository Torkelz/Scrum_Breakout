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

	void initApp();
	void onResize();
	void updateScene(float p_dt);
	void drawScene();
private:

};

#endif /* OPENGL_H_ */
