#ifndef OPENGL_H_
#define OPENGL_H_

#include "GLApp.h"
#include "LinHid.h"
#include "Game.h"
#include "GLTexture.h"
#include "../../GLUtility/Header/Buffer.h"
#include "../../GLUtility/Header/Shader.h"
#include <stdio.h>
#include <stdlib.h>

using namespace glm;
static int keyValueStatic;

class OpenGL : public GLApp{
	public:
	OpenGL();
	~OpenGL();

	void 			initApp();
	void 			updateScene(float p_dt);
	void 			drawScene();
	void			generatingKeyValue();
	void			copyMousePos();
	static void 	messageCallback(GLFWwindow* p_pMainWnd, int p_key, int p_scanCode, int p_action, int p_mods);

private:
	void			updateFPSCounter();

private:
	mat4x4 			m_camView;
	float			m_rotation;
	float			m_ratio;
	Buffer			triBuffer;
	Buffer			m_uniBuffer;
	Shader			m_triShader;
	HID*			m_pHID;
	//Game*			m_pGame;
	int 			m_keyValue;
	GLTexture*		m_pTexTest;
	GLuint			m_pTexture;

};

#endif /* OPENGL_H_ */
