//=============================================================================
// Init Direct3D.cpp by Frank Luna (C) 2008 All Rights Reserved.
//
// Demonstrates the sample framework by initializing Direct3D, clearing 
// the screen, and displaying frame stats.
//
//=============================================================================

#include "d3dApp.h"
#include <D3D11Shader.h>
#include <DirectXMath.h>
#include <DirectXColors.h>
#include <DirectXCollision.h>
#include <DirectXPackedVector.h>
#include <d3dCompiler.h>
#include <string>

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

private:
	XMMATRIX camView;
	XMMATRIX camProjection;
	
	XMVECTOR camPosition;
	XMVECTOR camTarget;
	XMVECTOR camUp;
};

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance,
				   PSTR cmdLine, int showCmd)
{
	// Enable run-time memory check for debug builds.
#if defined(DEBUG) | defined(_DEBUG)
	_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
#endif


	Direct3D theApp(hInstance);
	
	theApp.initApp();

	return theApp.run();
}

Direct3D::Direct3D(HINSTANCE hInstance)
: D3DApp(hInstance) 
{
}

Direct3D::~Direct3D()
{
	/*if( md3dDevice )
		md3dDevice->ClearState();*/
}

void Direct3D::initApp()
{
	D3DApp::initApp();
	HRESULT hr = S_OK;
	
	//Set up world view proj
	camPosition = XMVectorSet( 0.0f, 0.0f, -20.f, 0.0f );
	camTarget = XMVectorSet( 0.0f, 0.0f, 0.0f, 0.0f );
	camUp = XMVectorSet( 0.0f, 1.0f, 0.0f, 0.0f );

	camView = XMMatrixLookAtLH( camPosition, camTarget, camUp );

	camProjection = XMMatrixPerspectiveFovLH( 0.4f*3.14f, (float)mClientWidth/mClientHeight, 1.0f, 1000.0f);
}

void Direct3D::onResize()
{
	D3DApp::onResize();
}

void Direct3D::updateScene(float dt)
{
	D3DApp::updateScene(dt);
}

void Direct3D::drawScene()
{
	D3DApp::drawScene();

	mSwapChain->Present(0, 0);
}