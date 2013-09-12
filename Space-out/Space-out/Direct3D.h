#ifndef DIRECT3D_H
#define DIRECT3D_H

#include "d3dApp.h"
#include <DirectXPackedVector.h>
#include <string>

#include "Buffer.h"
#include "Shader.h"

#include "WinHID.h"
#include "Game.h"
#include "Observer.h"

class Object;

using namespace DirectX;
 
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
	XMMATRIX	m_camView;
	XMMATRIX	m_camProjection;
	XMMATRIX	m_world;
	XMMATRIX	m_WVP;
	XMVECTOR	m_camPosition;
	XMVECTOR	m_camTarget;
	XMVECTOR	m_camUp;

	HID			m_HID;
	Game		m_game;

	Buffer		m_buffer;
	Buffer		m_cBuffer;
	Shader		m_shader;
	//TEST
	ID3D11Buffer* mVB;
};
#endif