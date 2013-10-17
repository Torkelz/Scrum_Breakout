#include "d3dApp.h"
#include "Direct3D.h"

LRESULT CALLBACK
MainWndProc(HWND p_hwnd, UINT p_msg, WPARAM p_wParam, LPARAM p_lParam)
{
	static Direct3D* dd = 0;

	switch( p_msg )
	{
		case WM_CREATE:
		{
			// Get the 'this' pointer we passed to CreateWindow via the lpParam parameter.
			CREATESTRUCT* cs = (CREATESTRUCT*)p_lParam;
			dd = (Direct3D*)cs->lpCreateParams;
			return 0;
		}
	}
	// Don't start processing messages until after WM_CREATE.
	if( dd )
		return dd->msgProc(p_msg, p_wParam, p_lParam);
	else
		return DefWindowProc(p_hwnd, p_msg, p_wParam, p_lParam);
}

D3DApp::D3DApp(HINSTANCE p_hInstance)
{
	m_hAppInst   = p_hInstance;
	m_hMainWnd   = 0;
	m_AppPaused  = false;
	m_Minimized  = false;
	m_Maximized  = false;
	m_Resizing   = false;

	m_FrameStats = (LPCWSTR)"";
 
	m_pDevice			  = 0;
	m_pSwapChain          = 0;
	m_pDepthStencilBuffer = 0;
	m_pRenderTargetView   = 0;
	m_pDepthStencilView   = 0;

	m_MainWndTitle = (LPCWSTR)"Space-out";
	m_DriverType  = D3D_DRIVER_TYPE_HARDWARE;
	m_ClearColor[0] = 0.0f; m_ClearColor[1] = 0.0f; m_ClearColor[2] = 1.0f; m_ClearColor[3]  = 1.0f;
	m_ClientWidth    = 800;
	m_ClientHeight   = 600;

	m_4xMsaaQuality = 0;
	m_pDeviceContext = 0;

	m_winTitle = L"";
}

D3DApp::~D3DApp()
{
	ReleaseCOM(m_pRenderTargetView);
	ReleaseCOM(m_pDepthStencilView);
	ReleaseCOM(m_pSwapChain);
	ReleaseCOM(m_pDepthStencilBuffer);
	ReleaseCOM(m_pDeviceContext);
	ReleaseCOM(m_pDevice);
}

HINSTANCE D3DApp::getAppInst()
{
	return m_hAppInst;
}

HWND D3DApp::getMainWnd()
{
	return m_hMainWnd;
}

int D3DApp::run()
{
	MSG msg = {0};
 
	m_Timer.reset();

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
			m_Timer.tick();

			if( !m_AppPaused )
			{
				updateScene(m_Timer.getDeltaTime());
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
}
 
void D3DApp::onResize()
{
	// Release the old views, as they hold references to the buffers we
	// will be destroying.  Also release the old depth/stencil buffer.
	ReleaseCOM(m_pRenderTargetView);
	ReleaseCOM(m_pDepthStencilView);
	ReleaseCOM(m_pDepthStencilBuffer);

	// Resize the swap chain and recreate the render target view.
	m_pSwapChain->ResizeBuffers(1, m_ClientWidth, m_ClientHeight, DXGI_FORMAT_R8G8B8A8_UNORM, 0);
	ID3D11Texture2D* backBuffer;
	m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&backBuffer));
	m_pDevice->CreateRenderTargetView(backBuffer, 0, &m_pRenderTargetView);

	//IDXGISurface* pBackBuffer;
	//m_pSwapChain->GetBuffer( 0, IID_PPV_ARGS(&pBackBuffer) );
	//m_pTextDevice = new AdvancedText();
	//m_pTextDevice->Initialize(m_hMainWnd, pBackBuffer);

	ReleaseCOM(backBuffer);
	//ReleaseCOM(pBackBuffer);

	// Create the depth/stencil buffer and view.
	D3D11_TEXTURE2D_DESC depthStencilDesc;

	depthStencilDesc.Width     = m_ClientWidth;
	depthStencilDesc.Height    = m_ClientHeight;
	depthStencilDesc.MipLevels = 1;
	depthStencilDesc.ArraySize = 1;
	depthStencilDesc.Format    = DXGI_FORMAT_D24_UNORM_S8_UINT;

	// Use 4X MSAA? --must match swap chain MSAA values.
	if( m_Enable4xMsaa )
	{
		depthStencilDesc.SampleDesc.Count   = 4;
		depthStencilDesc.SampleDesc.Quality = m_4xMsaaQuality-1;
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

	m_pDevice->CreateTexture2D(&depthStencilDesc, 0, &m_pDepthStencilBuffer);
	m_pDevice->CreateDepthStencilView(m_pDepthStencilBuffer, 0, &m_pDepthStencilView);

	// Bind the render target view and depth/stencil view to the pipeline.
	m_pDeviceContext->OMSetRenderTargets(1, &m_pRenderTargetView, m_pDepthStencilView);

	// Set the viewport transform.
	m_ScreenViewport.TopLeftX = 0;
	m_ScreenViewport.TopLeftY = 0;
	m_ScreenViewport.Width    = static_cast<float>(m_ClientWidth);
	m_ScreenViewport.Height   = static_cast<float>(m_ClientHeight);
	m_ScreenViewport.MinDepth = 0.0f;
	m_ScreenViewport.MaxDepth = 1.0f;

	m_pDeviceContext->RSSetViewports(1, &m_ScreenViewport);
}

void D3DApp::updateScene(float p_dt)
{
	// Code computes the average frames per second, and also the 
	// average time it takes to render one frame.
	static int frameCnt = 0;
	static float t_base = 0.0f;

	frameCnt++;

	// Compute averages over one second period.
	if( (m_Timer.getGameTime() - t_base) >= 1.0f )
	{
		float fps = (float)frameCnt; // fps = frameCnt / 1
		float mspf = 1000.0f / fps;

		std::wostringstream outs;   
		outs.precision(6);
		outs << (LPCTSTR)m_MainWndTitle.c_str() << L"    "
			<< L"FPS: " << fps << L"    " 
			<< L"Frame Time: " << mspf << L" (ms)";


		m_winTitle = outs.str();
		//SetWindowTextW(m_hMainWnd, m_winTitle.c_str());

		// Reset for next average.
		frameCnt = 0;
		t_base  += 1.0f;
	}
}

void D3DApp::drawScene()
{
	m_pDeviceContext->ClearRenderTargetView(m_pRenderTargetView, m_ClearColor);
	m_pDeviceContext->ClearDepthStencilView(m_pDepthStencilView, D3D11_CLEAR_DEPTH|D3D11_CLEAR_STENCIL, 1.0f, 0);
}

LRESULT D3DApp::msgProc(UINT p_msg, WPARAM wParam, LPARAM lParam)
{
	switch( p_msg )
	{
	// WM_ACTIVATE is sent when the window is activated or deactivated.  
	// We pause the game when the window is deactivated and unpause it 
	// when it becomes active.  
	case WM_ACTIVATE:
		if( LOWORD(wParam) == WA_INACTIVE )
		{
			m_AppPaused = true;
			m_Timer.stop();
		}
		else
		{
			m_AppPaused = false;
			m_Timer.start();
		}
		return 0;

	// WM_SIZE is sent when the user resizes the window.  
	case WM_SIZE:
		// Save the new client area dimensions.
		m_ClientWidth  = LOWORD(lParam);
		m_ClientHeight = HIWORD(lParam);
		if( m_pDevice )
		{
			if( wParam == SIZE_MINIMIZED )
			{
				m_AppPaused = true;
				m_Minimized = true;
				m_Maximized = false;
			}
			else if( wParam == SIZE_MAXIMIZED )
			{
				m_AppPaused = false;
				m_Minimized = false;
				m_Maximized = true;
				onResize();
			}
			else if( wParam == SIZE_RESTORED )
			{
				
				// Restoring from minimized state?
				if( m_Minimized )
				{
					m_AppPaused = false;
					m_Minimized = false;
					onResize();
				}

				// Restoring from maximized state?
				else if( m_Maximized )
				{
					m_AppPaused = false;
					m_Maximized = false;
					onResize();
				}
				else if( m_Resizing )
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
				else // API call such as SetWindowPos or m_pSwapChain->SetFullscreenState.
				{
					onResize();
				}
			}
		}
		return 0;

	// WM_EXITSIZEMOVE is sent when the user grabs the resize bars.
	case WM_ENTERSIZEMOVE:
		m_AppPaused = true;
		m_Resizing  = true;
		m_Timer.stop();
		return 0;

	// WM_EXITSIZEMOVE is sent when the user releases the resize bars.
	// Here we reset everything based on the new window dimensions.
	case WM_EXITSIZEMOVE:
		m_AppPaused = false;
		m_Resizing  = false;
		m_Timer.start();
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
	}

	return DefWindowProc(m_hMainWnd, p_msg, wParam, lParam);
}


void D3DApp::initMainWindow()
{
	WNDCLASS wc;
	wc.style         = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc   = MainWndProc; 
	wc.cbClsExtra    = 0;
	wc.cbWndExtra    = 0;
	wc.hInstance     = m_hAppInst;
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
	RECT R = { 0, 0, m_ClientWidth, m_ClientHeight };
    AdjustWindowRect(&R, WS_OVERLAPPEDWINDOW, false);
	int width  = R.right - R.left;
	int height = R.bottom - R.top;

	m_hMainWnd = CreateWindow((LPCSTR)"D3DWndClassName",(LPCTSTR)m_MainWndTitle.c_str(), 
		WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, width, height, 0, 0, m_hAppInst, this); 
	if( !m_hMainWnd )
	{
		MessageBox(0, (LPCTSTR)"CreateWindow FAILED", 0, 0);
		PostQuitMessage(0);
	}

	ShowWindow(m_hMainWnd, SW_SHOW);
	UpdateWindow(m_hMainWnd);
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
		m_DriverType,
		0,                 // no software device
		createDeviceFlags, 
		0, 0,              // default feature level array
		D3D11_SDK_VERSION,
		&m_pDevice,
		&featureLevel,
		&m_pDeviceContext);

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

	m_pDevice->CheckMultisampleQualityLevels(
		DXGI_FORMAT_R8G8B8A8_UNORM, 4, &m_4xMsaaQuality);
	assert( m_4xMsaaQuality > 0 );

	// Fill out a DXGI_SWAP_CHAIN_DESC to describe our swap chain.
	DXGI_SWAP_CHAIN_DESC sd;
	sd.BufferDesc.Width  = m_ClientWidth;
	sd.BufferDesc.Height = m_ClientHeight;
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	// Use 4X MSAA? 
	if( m_Enable4xMsaa )
	{
		sd.SampleDesc.Count   = 4;
		sd.SampleDesc.Quality = m_4xMsaaQuality-1;
	}
	// No MSAA
	else
	{
		sd.SampleDesc.Count   = 1;
		sd.SampleDesc.Quality = 0;
	}

	sd.BufferUsage  = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.BufferCount  = 1;
	sd.OutputWindow = m_hMainWnd;
	sd.Windowed     = true;
	sd.SwapEffect   = DXGI_SWAP_EFFECT_DISCARD;
	sd.Flags        = 0;

	// To correctly create the swap chain, we must use the IDXGIFactory that was
	// used to create the device.  If we tried to use a different IDXGIFactory instance
	// (by calling CreateDXGIFactory), we get an error: "IDXGIFactory::CreateSwapChain: 
	// This function is being called with a device from a different IDXGIFactory."

	IDXGIDevice* dxgiDevice = 0;
	m_pDevice->QueryInterface(__uuidof(IDXGIDevice), (void**)&dxgiDevice);

	IDXGIAdapter* dxgiAdapter = 0;
	dxgiDevice->GetParent(__uuidof(IDXGIAdapter), (void**)&dxgiAdapter);

	IDXGIFactory* dxgiFactory = 0;
	dxgiAdapter->GetParent(__uuidof(IDXGIFactory), (void**)&dxgiFactory);

	dxgiFactory->CreateSwapChain(m_pDevice, &sd, &m_pSwapChain);

	ReleaseCOM(dxgiDevice);
	ReleaseCOM(dxgiAdapter);
	ReleaseCOM(dxgiFactory);

	// The remaining steps that need to be carried out for d3d creation
	// also need to be executed every time the window is resized.  So
	// just call the onResize method here to avoid code duplication.
	
	onResize();
}