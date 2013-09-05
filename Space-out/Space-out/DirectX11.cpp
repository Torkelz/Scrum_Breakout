#include "DirectX11.h"

//LRESULT CALLBACK
//MainWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
//{
//	return DefWindowProc(hwnd, msg, wParam, lParam);
//}

DirectX11::DirectX11(void)
{
}

DirectX11::~DirectX11(void)
{
}

void DirectX11::initApplication()
{
	initMainWindow();
	initGraphics();
}

void DirectX11::initMainWindow()
{
	WNDCLASS wc;
	wc.style         = CS_HREDRAW | CS_VREDRAW;
	//wc.lpfnWndProc   = MainWndProc; 
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

	m_hMainWnd = CreateWindow((LPCSTR)"D3DWndClassName",(LPCTSTR)m_MainWindowTitle.c_str(), 
		WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, width, height, 0, 0, m_hAppInst, this); 

	if( !m_hMainWnd )
	{
		MessageBox(0, (LPCTSTR)"CreateWindow FAILED", 0, 0);
		PostQuitMessage(0);
	}

	ShowWindow(m_hMainWnd, SW_SHOW);
	UpdateWindow(m_hMainWnd);
}

void DirectX11::initGraphics()
{
}

int DirectX11::run()
{
	return 0;
}

void DirectX11::onResize()
{
}

void DirectX11::updateScene()
{
}

void DirectX11::drawScene()
{
}

void DirectX11::getAppInst()
{
}

void DirectX11::getMainWindow()
{
}
