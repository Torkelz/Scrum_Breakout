#ifndef DIRECT3D_H
#define DIRECT3D_H

#include "d3dApp.h"
#include <DirectXPackedVector.h>
#include <string>

#include "WinHID.h"
#include "Game.h"
#include "Observer.h"

using namespace DirectX;
 
class Direct3D : public D3DApp
{
public:
	Direct3D(HINSTANCE hInstance);
	~Direct3D();

	void initApp();
	void onResize();
	void updateScene(float dt);
	void drawScene();

	LRESULT msgProc(UINT msg, WPARAM wParam, LPARAM lParam);
private:
	//Camera Variables
	XMMATRIX camView;
	XMMATRIX camProjection;
	
	XMVECTOR camPosition;
	XMVECTOR camTarget;
	XMVECTOR camUp;

	int test;

	HID			m_HID;
	Game		m_game;
};
#endif