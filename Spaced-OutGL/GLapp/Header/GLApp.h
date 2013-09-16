#ifndef GLAPP_H_
#define GLAPP_H_

#include "GLUtil.h"

using namespace std;

class GLApp
{
public:
	GLApp();
	virtual ~GLApp();

	GLFWwindow*      getMainWnd();

	int run();

	// Framework methods.  Derived client class overrides these methods to
	// implement specific application requirements.

	virtual void		initApp();
	virtual void		onResize();// reset projection/etc
	virtual void		updateScene(float p_dt);
	virtual void		drawScene();

protected:
	void initMainWindow();
	void initOpenGL();

protected:

	GLFWwindow*					m_hMainWnd;
	bool						m_AppPaused;
	bool						m_Minimized;
	bool						m_Maximized;
	bool						m_Resizing;

	//GameTimer					m_Timer;

	//std::wstring				m_FrameStats;

	//ID3D11Device*				m_pDevice;
	//ID3D11DeviceContext*		m_pDeviceContext;
	//IDXGISwapChain*				m_pSwapChain;
	//ID3D11Texture2D*			m_pDepthStencilBuffer;
	//ID3D11RenderTargetView*		m_pRenderTargetView;
	//ID3D11DepthStencilView*		m_pDepthStencilView;

	//D3D11_VIEWPORT				m_ScreenViewport;

	// Derived class should set these in derived constructor to customize starting values.
	string						m_MainWndTitle;
	//D3D_DRIVER_TYPE			m_DriverType;
	//float						m_ClearColor[4];
	int							m_ClientWidth;
	int							m_ClientHeight;
	//bool						m_Enable4xMsaa;

};
#endif /* GLAPP_H_ */
