#ifndef DIRECTX11_H
#define DIRECTX11_H
#include "graphics.h"
#include "d3dUtil.h"


class DirectX11 : public Graphics
{
public:
	DirectX11(void);
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
};

#endif