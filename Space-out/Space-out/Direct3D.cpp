#include "Direct3D.h"

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
	ReleaseCOM(m_pDevice);
	ReleaseCOM(m_pDeviceContext);
	ReleaseCOM(m_pSwapChain);
	ReleaseCOM(m_pDepthStencilBuffer);
	ReleaseCOM(m_pRenderTargetView);
	ReleaseCOM(m_pDepthStencilView);
}

void Direct3D::initApp()
{
	D3DApp::initApp();
	HRESULT hr = S_OK;

	m_HID = HID( getMainWnd() );
	
	//Set up world view projdf
	camPosition = XMVectorSet( 0.0f, 0.0f, -20.f, 0.0f );
	camTarget = XMVectorSet( 0.0f, 0.0f, 0.0f, 0.0f );
	camUp = XMVectorSet( 0.0f, 1.0f, 0.0f, 0.0f );

	camView = XMMatrixLookAtLH( camPosition, camTarget, camUp );

	camProjection = XMMatrixPerspectiveFovLH( 0.4f*3.14f, (float)m_ClientWidth/m_ClientHeight, 1.0f, 1000.0f);

	test = 10;

	m_game = Game();
	m_game.init();

	// Add subscriber to the HID component. 
	m_HID.getObservable()->addSubscriber(m_game.getObserver());
}

void Direct3D::onResize()
{
	D3DApp::onResize();
}

void Direct3D::updateScene(float dt)
{
	D3DApp::updateScene(dt);
	m_game.update();
}

void Direct3D::drawScene()
{
	D3DApp::drawScene();

	m_pSwapChain->Present(0, 0);
}

LRESULT Direct3D::msgProc(UINT msg, WPARAM wParam, LPARAM lParam)
{
	m_HID.update(msg, lParam);
	/*switch( msg )
	{
		return 0;
	}*/

	return D3DApp::msgProc(msg, wParam, lParam);
}