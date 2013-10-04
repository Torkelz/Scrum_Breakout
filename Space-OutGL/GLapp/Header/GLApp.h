#ifndef GLAPP_H_
#define GLAPP_H_

#include "GLUtil.h"
#include <unistd.h>
#include <sstream>

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
	virtual void		updateScene(float p_dt);
	virtual void		drawScene();
	static  void		errorCallback(int p_error, const char* p_pDescription);
//	static 	void		resizeCallback(GLFWwindow* p_pMainWnd, int p_width, int p_height);
//	static	void		MessageLoop(GLFWwindow* p_pMainWnd, int p_key, int p_scanCode, int p_action, int p_mods);
private:
	static 	bool 		gl_log(const char* p_pMessage, const char* p_pFilename, int p_line);
protected:
	void 				initMainWindow();
	void 				initOpenGL();

protected:

	GLFWwindow*					m_hMainWnd;
	bool						m_appPaused;
	bool						m_minimized;
	bool						m_maximized;
	bool						m_resizing;

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