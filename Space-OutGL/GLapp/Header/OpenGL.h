#ifndef OPENGL_H_
#define OPENGL_H_

#include "GLApp.h"
#include "LinHid.h"
#include "Game.h"
#include "GLTexture.h"
#include "../../GLUtility/Header/Buffer.h"
#include "../../GLUtility/Header/Shader.h"
#include "Camera.h"
#include "PUObserver.h"
#include <stdio.h>
#include <stdlib.h>

struct SuniBlock
{
	mat4 proj;
};

struct SuniBall
{
	mat4 Trans;
	mat4 projView;
	vec4 eyepos;
	vec4 size;
};

struct SuniPad
{
	mat4 PVW;
};

struct padVertex
{
	vec3 m_position;
	vec2 m_texCoord;
};

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
	void 			addPowerUp(PowerUp* p_pPowerUp);
	void 			removePowerUp(int i);

private:
	void			updateFPSCounter();

private:
	PUObserver*		m_pPUObserver;
	vector<PowerUp*> m_powerUps;
	SuniBlock m_uniBlock[1];
	mat4x4 			m_camView;
	float			m_rotation;
	float			m_ratio;
	Buffer			m_uniBlockBuffer;
	Shader			m_triShader;
	HID*			m_pHID;
	Game			m_game;
	int 			m_keyValue;
	GLTexture*		m_pTexBlock;
	GLuint			m_pBlockPic;

	Camera*         m_pCamera;
	float m_scale;
	Buffer			m_blockBuffer[4];

	//BALL
	SuniBall 		m_uniBall[1];
	Buffer			m_ballBuffer;
	Buffer			m_uniBallbuffer;
	Shader			m_ballShader;
	GLTexture*		m_pTexBall;
	GLuint			m_pBallPic;

	//PAD
	SuniPad			m_uniPad[1];
	Buffer			m_padBuffer;
	Buffer			m_uniPadbuffer;
	Shader			m_padShader;
	GLTexture*		m_pTexPad;
	GLuint			m_pPadPic;

};

#endif /* OPENGL_H_ */