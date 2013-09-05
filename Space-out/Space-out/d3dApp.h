//=======================================================================================
// d3dApp.h by Frank Luna (C) 2008 All Rights Reserved.
//
// Simple Direct3D demo application class.  
// Make sure you link: d3d10.lib d3dx10d.lib dxerr.lib dxguid.lib.
// Link d3dx10.lib for release mode builds instead of d3dx10d.lib.
//=======================================================================================


#ifndef D3DAPP_H
#define D3DAPP_H


#include "d3dUtil.h"
#include "GameTimer.h"
#include <string>


class D3DApp
{
public:
	D3DApp(HINSTANCE hInstance);
	virtual ~D3DApp();

	HINSTANCE getAppInst();
	HWND      getMainWnd();

	//void resetOMTargetsAndViewport();

	int run();

	// Framework methods.  Derived client class overrides these methods to 
	// implement specific application requirements.

	virtual void initApp();
	virtual void onResize();// reset projection/etc
	virtual void updateScene(float dt);
	virtual void drawScene(); 
	virtual LRESULT msgProc(UINT msg, WPARAM wParam, LPARAM lParam);

protected:
	void initMainWindow();
	void initDirect3D();
	
protected:

	HINSTANCE mhAppInst;
	HWND      mhMainWnd;
	bool      mAppPaused;
	bool      mMinimized;
	bool      mMaximized;
	bool      mResizing;

	UINT      m4xMsaaQuality;

	GameTimer mTimer;

	std::wstring mFrameStats;
 
	ID3D11Device*    md3dDevice;
	ID3D11DeviceContext* md3dImmediateContext;
	IDXGISwapChain*  mSwapChain;
	ID3D11Texture2D* mDepthStencilBuffer;
	ID3D11RenderTargetView* mRenderTargetView;
	ID3D11DepthStencilView* mDepthStencilView;

	D3D11_VIEWPORT mScreenViewport;

	//ID3DX10Font* mFont;

	// Derived class should set these in derived constructor to customize starting values.
	std::wstring mMainWndCaption;
	D3D_DRIVER_TYPE md3dDriverType;
	float mClearColor[4];
	int mClientWidth;
	int mClientHeight;
	bool mEnable4xMsaa;
	
};




#endif // D3DAPP_H