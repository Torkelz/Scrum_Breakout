//=======================================================================================
// d3dApp.cpp by Frank Luna (C) 2008 All Rights Reserved.
//=======================================================================================

#include "d3dApp.h"
#include <sstream>

LRESULT CALLBACK
MainWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	static D3DApp* app = 0;

	switch( msg )
	{
		case WM_CREATE:
		{
			// Get the 'this' pointer we passed to CreateWindow via the lpParam parameter.
			CREATESTRUCT* cs = (CREATESTRUCT*)lParam;
			app = (D3DApp*)cs->lpCreateParams;
			return 0;
		}
	}

	// Don't start processing messages until after WM_CREATE.
	if( app )
		return app->msgProc(msg, wParam, lParam);
	else
		return DefWindowProc(hwnd, msg, wParam, lParam);
}

D3DApp::D3DApp(HINSTANCE hInstance)
{
	mhAppInst   = hInstance;
	mhMainWnd   = 0;
	mAppPaused  = false;
	mMinimized  = false;
	mMaximized  = false;
	mResizing   = false;

	mFrameStats = (LPCWSTR)"";
 
	md3dDevice          = 0;
	mSwapChain          = 0;
	mDepthStencilBuffer = 0;
	mRenderTargetView   = 0;
	mDepthStencilView   = 0;
	//mFont               = 0;

	mMainWndCaption = (LPCWSTR)"Space-out";
	md3dDriverType  = D3D_DRIVER_TYPE_HARDWARE;
	mClearColor[0] = 0.0f; mClearColor[1] = 0.0f; mClearColor[2] = 1.0f; mClearColor[3]  = 1.0f;
	mClientWidth    = 800;
	mClientHeight   = 600;

	m4xMsaaQuality = 0;
	md3dImmediateContext = 0;

	m_buffer = Buffer();
}

D3DApp::~D3DApp()
{
	ReleaseCOM(mRenderTargetView);
	ReleaseCOM(mDepthStencilView);
	ReleaseCOM(mSwapChain);
	ReleaseCOM(mDepthStencilBuffer);

	// Restore all default settings.
	if( md3dImmediateContext )
		md3dImmediateContext->ClearState();

	ReleaseCOM(md3dImmediateContext);
	ReleaseCOM(md3dDevice);
	//ReleaseCOM(mFont);
}

HINSTANCE D3DApp::getAppInst()
{
	return mhAppInst;
}

HWND D3DApp::getMainWnd()
{
	return mhMainWnd;
}

//void D3DApp::resetOMTargetsAndViewport()
//{
//	md3dDevice->OMSetRenderTargets(1, &mRenderTargetView, mDepthStencilView);
//
//	D3D10_VIEWPORT vp;
//	vp.TopLeftX = 0;
//	vp.TopLeftY = 0;
//	vp.Width    = mClientWidth;
//	vp.Height   = mClientHeight;
//	vp.MinDepth = 0.0f;
//	vp.MaxDepth = 1.0f;
//
//	md3dDevice->RSSetViewports(1, &vp);
//}

int D3DApp::run()
{
	MSG msg = {0};
 
	mTimer.reset();

	while(msg.message != WM_QUIT)
	{
		// If there are Window messages then process them.
		if(PeekMessage( &msg, 0, 0, 0, PM_REMOVE ))
		{
            TranslateMessage( &msg );
            DispatchMessage( &msg );
		}
		// Otherwise, do animation/game stuff.
		else
        {	
			mTimer.tick();

			if( !mAppPaused )
			{
				updateScene(mTimer.getDeltaTime());
				drawScene();
			}
			else
				Sleep(100);

			
        }
    }
	return (int)msg.wParam;
}

void D3DApp::initApp()
{
	initMainWindow();
	initDirect3D();

	//TESTING THE BUFFER
	//VertexTest vertexTests[] =
	//{
	//	DirectX::XMFLOAT3( 0.0f, 0.5f, 0.5f ),
	//	DirectX::XMFLOAT3( 0.0f, 0.0f, 0.5f ),
	//	DirectX::XMFLOAT3( 0.5f, -0.5f, 0.5f ),
	//	DirectX::XMFLOAT3( 0.5f, 0.0f, 0.0f ),
	//	DirectX::XMFLOAT3( -0.5f, -0.5f, 0.5f ),
	//	DirectX::XMFLOAT3( 0.0f, 0.5f, 0.0f ),
	//};

	//BufferInitDesc initDesc;

	//initDesc.elementSize = sizeof(VertexTest);
	//initDesc.numElements = sizeof(vertexTest) / sizeof(VertexTest);
	//initDesc.type = VERTEX_BUFFER;
	//initDesc.usage = BUFFER_DEFAULT;
	//initDesc.initData = vertexTest;

	//m_buffer.init(md3dDevice, md3dImmediateContext, initDesc);
	

	/*D3DX10_FONT_DESC fontDesc;
	fontDesc.Height          = 24;
	fontDesc.Width           = 0;
	fontDesc.Weight          = 0;
	fontDesc.MipLevels       = 1;
	fontDesc.Italic          = false;
	fontDesc.CharSet         = DEFAULT_CHARSET;
	fontDesc.OutputPrecision = OUT_DEFAULT_PRECIS;
	fontDesc.Quality         = DEFAULT_QUALITY;
	fontDesc.PitchAndFamily  = DEFAULT_PITCH | FF_DONTCARE;
	wcscpy_s((wchar_t *)fontDesc.FaceName, 5, (LPCWSTR)"Arial");

	D3DX10CreateFontIndirect(md3dDevice, &fontDesc, &mFont);*/
}
 
void D3DApp::onResize()
{
	// Release the old views, as they hold references to the buffers we
	// will be destroying.  Also release the old depth/stencil buffer.

	ReleaseCOM(mRenderTargetView);
	ReleaseCOM(mDepthStencilView);
	ReleaseCOM(mDepthStencilBuffer);


	// Resize the swap chain and recreate the render target view.

	mSwapChain->ResizeBuffers(1, mClientWidth, mClientHeight, DXGI_FORMAT_R8G8B8A8_UNORM, 0);
	ID3D11Texture2D* backBuffer;
	mSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&backBuffer));
	md3dDevice->CreateRenderTargetView(backBuffer, 0, &mRenderTargetView);
	ReleaseCOM(backBuffer);


	// Create the depth/stencil buffer and view.

	D3D11_TEXTURE2D_DESC depthStencilDesc;

	depthStencilDesc.Width     = mClientWidth;
	depthStencilDesc.Height    = mClientHeight;
	depthStencilDesc.MipLevels = 1;
	depthStencilDesc.ArraySize = 1;
	depthStencilDesc.Format    = DXGI_FORMAT_D24_UNORM_S8_UINT;

	// Use 4X MSAA? --must match swap chain MSAA values.
	if( mEnable4xMsaa )
	{
		depthStencilDesc.SampleDesc.Count   = 4;
		depthStencilDesc.SampleDesc.Quality = m4xMsaaQuality-1;
	}
	// No MSAA
	else
	{
		depthStencilDesc.SampleDesc.Count   = 1;
		depthStencilDesc.SampleDesc.Quality = 0;
	}

	depthStencilDesc.Usage          = D3D11_USAGE_DEFAULT;
	depthStencilDesc.BindFlags      = D3D11_BIND_DEPTH_STENCIL;
	depthStencilDesc.CPUAccessFlags = 0; 
	depthStencilDesc.MiscFlags      = 0;

	md3dDevice->CreateTexture2D(&depthStencilDesc, 0, &mDepthStencilBuffer);
	md3dDevice->CreateDepthStencilView(mDepthStencilBuffer, 0, &mDepthStencilView);


	// Bind the render target view and depth/stencil view to the pipeline.

	md3dImmediateContext->OMSetRenderTargets(1, &mRenderTargetView, mDepthStencilView);


	// Set the viewport transform.

	mScreenViewport.TopLeftX = 0;
	mScreenViewport.TopLeftY = 0;
	mScreenViewport.Width    = static_cast<float>(mClientWidth);
	mScreenViewport.Height   = static_cast<float>(mClientHeight);
	mScreenViewport.MinDepth = 0.0f;
	mScreenViewport.MaxDepth = 1.0f;

	md3dImmediateContext->RSSetViewports(1, &mScreenViewport);
}

void D3DApp::updateScene(float dt)
{
	// Code computes the average frames per second, and also the 
	// average time it takes to render one frame.

	static int frameCnt = 0;
	static float t_base = 0.0f;

	frameCnt++;

	// Compute averages over one second period.
	if( (mTimer.getGameTime() - t_base) >= 1.0f )
	{
		float fps = (float)frameCnt; // fps = frameCnt / 1
		float mspf = 1000.0f / fps;

		std::wostringstream outs;   
		outs.precision(6);
		outs << (LPCTSTR)mMainWndCaption.c_str() << L"    "
			<< L"FPS: " << fps << L"    " 
			<< L"Frame Time: " << mspf << L" (ms)";

		SetWindowTextW(mhMainWnd, outs.str().c_str());

		// Reset for next average.
		frameCnt = 0;
		t_base  += 1.0f;
	}
}

void D3DApp::drawScene()
{
	md3dImmediateContext->ClearRenderTargetView(mRenderTargetView, mClearColor);
	md3dImmediateContext->ClearDepthStencilView(mDepthStencilView, D3D11_CLEAR_DEPTH|D3D11_CLEAR_STENCIL, 1.0f, 0);
}

LRESULT D3DApp::msgProc(UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch( msg )
	{
	// WM_ACTIVATE is sent when the window is activated or deactivated.  
	// We pause the game when the window is deactivated and unpause it 
	// when it becomes active.  
	case WM_ACTIVATE:
		if( LOWORD(wParam) == WA_INACTIVE )
		{
			mAppPaused = true;
			mTimer.stop();
		}
		else
		{
			mAppPaused = false;
			mTimer.start();
		}
		return 0;

	// WM_SIZE is sent when the user resizes the window.  
	case WM_SIZE:
		// Save the new client area dimensions.
		mClientWidth  = LOWORD(lParam);
		mClientHeight = HIWORD(lParam);
		if( md3dDevice )
		{
			if( wParam == SIZE_MINIMIZED )
			{
				mAppPaused = true;
				mMinimized = true;
				mMaximized = false;
			}
			else if( wParam == SIZE_MAXIMIZED )
			{
				mAppPaused = false;
				mMinimized = false;
				mMaximized = true;
				onResize();
			}
			else if( wParam == SIZE_RESTORED )
			{
				
				// Restoring from minimized state?
				if( mMinimized )
				{
					mAppPaused = false;
					mMinimized = false;
					onResize();
				}

				// Restoring from maximized state?
				else if( mMaximized )
				{
					mAppPaused = false;
					mMaximized = false;
					onResize();
				}
				else if( mResizing )
				{
					// If user is dragging the resize bars, we do not resize 
					// the buffers here because as the user continuously 
					// drags the resize bars, a stream of WM_SIZE messages are
					// sent to the window, and it would be pointless (and slow)
					// to resize for each WM_SIZE message received from dragging
					// the resize bars.  So instead, we reset after the user is 
					// done resizing the window and releases the resize bars, which 
					// sends a WM_EXITSIZEMOVE message.
				}
				else // API call such as SetWindowPos or mSwapChain->SetFullscreenState.
				{
					onResize();
				}
			}
		}
		return 0;

	// WM_EXITSIZEMOVE is sent when the user grabs the resize bars.
	case WM_ENTERSIZEMOVE:
		mAppPaused = true;
		mResizing  = true;
		mTimer.stop();
		return 0;

	// WM_EXITSIZEMOVE is sent when the user releases the resize bars.
	// Here we reset everything based on the new window dimensions.
	case WM_EXITSIZEMOVE:
		mAppPaused = false;
		mResizing  = false;
		mTimer.start();
		onResize();
		return 0;
 
	// WM_DESTROY is sent when the window is being destroyed.
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;

	// The WM_MENUCHAR message is sent when a menu is active and the user presses 
	// a key that does not correspond to any mnemonic or accelerator key. 
	case WM_MENUCHAR:
        // Don't beep when we alt-enter.
        return MAKELRESULT(0, MNC_CLOSE);
 
	// Catch this message so to prevent the window from becoming too small.
	case WM_GETMINMAXINFO:
		((MINMAXINFO*)lParam)->ptMinTrackSize.x = 200;
		((MINMAXINFO*)lParam)->ptMinTrackSize.y = 200; 
		return 0;

	case WM_KEYDOWN:

		switch(wParam)
		{
		case VK_ESCAPE:
			PostQuitMessage(0);
			break;
		}
		return 0;
	}

	return DefWindowProc(mhMainWnd, msg, wParam, lParam);
}


void D3DApp::initMainWindow()
{
	WNDCLASS wc;
	wc.style         = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc   = MainWndProc; 
	wc.cbClsExtra    = 0;
	wc.cbWndExtra    = 0;
	wc.hInstance     = mhAppInst;
	wc.hIcon         = LoadIcon(0, IDI_APPLICATION);
	wc.hCursor       = LoadCursor(0, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(NULL_BRUSH);
	wc.lpszMenuName  = 0;
	wc.lpszClassName = (LPCTSTR)"D3DWndClassName";

	if( !RegisterClass(&wc) )
	{
		MessageBox(0, (LPCTSTR)"RegisterClass FAILED", 0, 0);
		PostQuitMessage(0);
	}

	// Compute window rectangle dimensions based on requested client area dimensions.
	RECT R = { 0, 0, mClientWidth, mClientHeight };
    AdjustWindowRect(&R, WS_OVERLAPPEDWINDOW, false);
	int width  = R.right - R.left;
	int height = R.bottom - R.top;

	mhMainWnd = CreateWindow((LPCSTR)"D3DWndClassName",(LPCTSTR)mMainWndCaption.c_str(), 
		WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, width, height, 0, 0, mhAppInst, this); 
	if( !mhMainWnd )
	{
		MessageBox(0, (LPCTSTR)"CreateWindow FAILED", 0, 0);
		PostQuitMessage(0);
	}

	ShowWindow(mhMainWnd, SW_SHOW);
	UpdateWindow(mhMainWnd);
}

void D3DApp::initDirect3D()
{
	

	// Create the device.

	UINT createDeviceFlags = 0;
	//#if defined(DEBUG) || defined(_DEBUG)  
	//    createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
	//#endif

	D3D_FEATURE_LEVEL featureLevel;
	HRESULT hr = D3D11CreateDevice(
		0,                 // default adapter
		md3dDriverType,
		0,                 // no software device
		createDeviceFlags, 
		0, 0,              // default feature level array
		D3D11_SDK_VERSION,
		&md3dDevice,
		&featureLevel,
		&md3dImmediateContext);

	if( FAILED(hr) )
	{
		MessageBox(0, (LPCTSTR)"D3D11CreateDevice Failed.", 0, 0);
		PostQuitMessage(0);
	}

	if( featureLevel != D3D_FEATURE_LEVEL_11_0 )
	{
		MessageBox(0, (LPCTSTR)"Direct3D Feature Level 11 unsupported.", 0, 0);
		PostQuitMessage(0);
	}

	// Check 4X MSAA quality support for our back buffer format.
	// All Direct3D 11 capable devices support 4X MSAA for all render 
	// target formats, so we only need to check quality support.

	md3dDevice->CheckMultisampleQualityLevels(
		DXGI_FORMAT_R8G8B8A8_UNORM, 4, &m4xMsaaQuality);
	assert( m4xMsaaQuality > 0 );

	// Fill out a DXGI_SWAP_CHAIN_DESC to describe our swap chain.

	DXGI_SWAP_CHAIN_DESC sd;
	sd.BufferDesc.Width  = mClientWidth;
	sd.BufferDesc.Height = mClientHeight;
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	// Use 4X MSAA? 
	if( mEnable4xMsaa )
	{
		sd.SampleDesc.Count   = 4;
		sd.SampleDesc.Quality = m4xMsaaQuality-1;
	}
	// No MSAA
	else
	{
		sd.SampleDesc.Count   = 1;
		sd.SampleDesc.Quality = 0;
	}

	sd.BufferUsage  = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.BufferCount  = 1;
	sd.OutputWindow = mhMainWnd;
	sd.Windowed     = true;
	sd.SwapEffect   = DXGI_SWAP_EFFECT_DISCARD;
	sd.Flags        = 0;


	// To correctly create the swap chain, we must use the IDXGIFactory that was
	// used to create the device.  If we tried to use a different IDXGIFactory instance
	// (by calling CreateDXGIFactory), we get an error: "IDXGIFactory::CreateSwapChain: 
	// This function is being called with a device from a different IDXGIFactory."

	IDXGIDevice* dxgiDevice = 0;
	md3dDevice->QueryInterface(__uuidof(IDXGIDevice), (void**)&dxgiDevice);

	IDXGIAdapter* dxgiAdapter = 0;
	dxgiDevice->GetParent(__uuidof(IDXGIAdapter), (void**)&dxgiAdapter);

	IDXGIFactory* dxgiFactory = 0;
	dxgiAdapter->GetParent(__uuidof(IDXGIFactory), (void**)&dxgiFactory);

	dxgiFactory->CreateSwapChain(md3dDevice, &sd, &mSwapChain);

	ReleaseCOM(dxgiDevice);
	ReleaseCOM(dxgiAdapter);
	ReleaseCOM(dxgiFactory);


	// The remaining steps that need to be carried out for d3d creation
	// also need to be executed every time the window is resized.  So
	// just call the onResize method here to avoid code duplication.
	
	onResize();
}


