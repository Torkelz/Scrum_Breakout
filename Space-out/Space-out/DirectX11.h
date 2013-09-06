#ifndef DIRECTX11_H
#define DIRECTX11_H
#include "graphics.h"
#include "d3dUtil.h"


class DirectX11 : public Graphics
{
public:
	DirectX11(HINSTANCE p_hInstance);
	~DirectX11(void);
	void initApplication();
	void initMainWindow();
	void initGraphics();
	int run();
	void onResize();
	void updateScene();
	void drawScene();
	
	void getAppInst(); //Placeholder, check type for DX11
	void getMainWindow(); //Placeholder, check type for DX11
private:
	HINSTANCE m_hAppInst;
	HWND      m_hMainWnd;
	ID3D11Device* m_Device;
	ID3D11DeviceContext* m_DeviceContext;
	IDXGISwapChain*	m_SwapChain;
	D3D_DRIVER_TYPE m_DriverType;
	bool m_Enable4xMsaa;
	UINT m_4xMsaaQuality;
};

#endif