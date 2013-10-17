#ifndef DIRECT3D_H
#define DIRECT3D_H

#include "d3dApp.h"
#include <string>

#include "Buffer.h"
#include "Shader.h"

#include "WinHID.h"
#include "Game.h"
#include "Observer.h"

#include "BoundingVolume.h"
#include "AABB.h"
#include "D3DTexture.h"
#include "PUObserver.h"
#include "Camera.h"
#include "SkyBox.h"

#include "D3DTextDevice.h"

class Object;

using namespace DirectX;

struct Vertex
{
	vec3 m_position;
	vec2 m_textureCoordinates;
};

struct CBPad
{
	XMMATRIX WVP;
};

struct CBBall
{
	XMVECTOR eyePosW;
	XMMATRIX viewProj;
	XMMATRIX translation;
	XMFLOAT2 size;
};

inline XMMATRIX mat4ToXMMatrix(mat4 p)
{
	XMMATRIX r;

	for (int i = 0; i < 4; i++)
	{
		r.r[i].m128_f32[0] = p[i].x;
		r.r[i].m128_f32[1] = p[i].y;
		r.r[i].m128_f32[2] = p[i].z;
		r.r[i].m128_f32[3] = p[i].w;
	}

	return r;
};
inline mat4 XMMatrixTomat4(XMMATRIX* p)
{
	mat4 r;

	for (int i = 0; i < 4; i++)
	{
		r[i].x = p->r[i].m128_f32[0];
		r[i].y = p->r[i].m128_f32[1];
		r[i].z = p->r[i].m128_f32[2];
		r[i].w = p->r[i].m128_f32[3];
	}

	return r;
};

inline std::string IntToString(int i)
{
	std::stringstream ss;
	ss << i;
	return ss.str();
};
inline XMVECTOR vec3ToXMVector(vec3 p)
{
	XMVECTOR r;
	r = XMVectorSet(p.x, p.y, p.z, 0.0f);

	return r;
};

class Direct3D : public D3DApp
{
public:
	Direct3D(HINSTANCE p_hInstance);
	~Direct3D();

	void initApp();
	void onResize();
	void updateScene(float p_dt);
	void drawScene();

	LRESULT			msgProc(UINT p_msg, WPARAM p_wParam, LPARAM p_lParam);
	void			addPowerUp(PowerUp* p_pPowerUp);
	void			removePowerUp(int i);
private:
	//Camera Variables
	XMMATRIX		 m_camView;
	XMMATRIX		 m_camProjection;
	XMMATRIX		 m_world;
	XMMATRIX		 m_WVP;
	XMVECTOR		 m_camPosition;
	XMVECTOR		 m_camTarget;
	XMVECTOR		 m_camUp;
					 
	HID				 m_HID;
	Game			 m_game;

	//Block Test
	
	Buffer			 m_blockBuffers[4];
	Shader			 m_blockShader;
	Buffer			 m_cBlockBuffer;
	vector<ABlock*>* m_pBlockList;
	int				 m_blockBufferSizeF;
	int				 m_blockBufferSizeB;
	int				 m_blockBufferSizeL;
	int				 m_blockBufferSizeR;
	D3DTexture		 m_blockTexture;
	Camera*			 m_pCamera;

	//border
	Buffer			m_borderBuffers;
	D3DTexture		m_borderTexture;
	Shader			 m_borderShader;
		

	//Pad
	Buffer			m_buffer;
	Buffer			m_cBuffer;
	Shader			m_shader;
	CBPad			m_cbPad;
	D3DTexture		m_padTexture;

	//Ball
	Buffer		m_ballBuffer;
	Buffer		m_constantBallBuffer;
	Shader		m_ballShader;
	CBBall		m_cbBall;
	D3DTexture	m_ballTexture;
	ID3D11SamplerState* m_pBallSampler;

	//Skybox
	SkyBox*		m_skyBox;
	Buffer*		m_skyBoxVbuffer;
	Buffer*		m_skyBoxIbuffer;
	Buffer		m_constantSkyBuffer;
	Shader		m_skyBoxShader;
	D3DTexture	m_skyTexture;
	ID3D11ShaderResourceView* m_skysrv;
	ID3D11SamplerState* m_pSkySampler;
	ID3D11RasterizerState* m_pRasterState;

	//TEST
	ID3D11Buffer*    mVB;

	//POWERUP
	PUObserver*		 m_pPUObserver;
	vector<PowerUp*> m_powerUps;
	Buffer			 m_powerBuffer;
	D3DTexture		 m_powerTextures[10];
	Shader			 m_powerShader;
	ID3D11BlendState* m_pPowerBlend;
	// DEBUGGING DRAW

	// TEXT TEST
	D3DTextDevice*	 m_pTextDevice;

};
#endif