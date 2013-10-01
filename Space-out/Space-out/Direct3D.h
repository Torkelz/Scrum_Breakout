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

class Object;

using namespace DirectX;
 
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

class Direct3D : public D3DApp
{
public:
	Direct3D(HINSTANCE p_hInstance);
	~Direct3D();

	void initApp();
	void onResize();
	void updateScene(float p_dt);
	void drawScene();

	LRESULT msgProc(UINT p_msg, WPARAM p_wParam, LPARAM p_lParam);
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

	Buffer			 m_buffer;
	Buffer			 m_cBuffer;
	Shader			 m_shader;
	CBPad		m_cbPad;

	//Ball
	Buffer		m_ballBuffer;
	Buffer		m_constantBallBuffer;
	Shader		m_ballShader;
	CBBall		m_cbBall;
	D3DTexture	m_ballTexture;
	ID3D11SamplerState* m_pBallSampler;

	//TEST
	ID3D11Buffer*    mVB;

	// DEBUGGING DRAW
	
};
#endif