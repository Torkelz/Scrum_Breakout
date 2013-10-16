#pragma once

#include "d3dUtil.h"
#include <d2d1.h>
#include <dwrite.h>

#ifndef HINST_THISCOMPONENT
EXTERN_C IMAGE_DOS_HEADER __ImageBase;
#define HINST_THISCOMPONENT ((HINSTANCE)&__ImageBase)
#endif

class AdvancedText
{
public: // ## FUNCTIONS ##
	AdvancedText();
    ~AdvancedText();

    HRESULT Initialize(HWND hwndParent, IDXGISurface* p_RT);
    HWND GetHwnd() { return hwnd_; }
	
    static LRESULT CALLBACK WndProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam );
    HRESULT DrawD2DContent();
    HRESULT DrawText();

private: // ## FUNCTIONS ##
    HRESULT CreateDeviceIndependentResources();
    void DiscardDeviceIndependentResources();
    HRESULT CreateDeviceResources(IDXGISurface* p_RT);
    void DiscardDeviceResources();

    void OnResize( UINT width, UINT height );

private: // ## VARIABLES ##
    HWND hwnd_;

    // how much to scale a design that assumes 96-DPI pixels
    float					dpiX;
    float					dpiY;

    // Direct2D
    ID2D1Factory*			pD2DFactory_;
	ID2D1HwndRenderTarget*	pRT_;
    //ID2D1RenderTarget*		pRT_;
    ID2D1SolidColorBrush*	pBlackBrush_;

    // DirectWrite
    IDWriteFactory*			pDWriteFactory_;
    IDWriteTextFormat*		pTextFormat_;

    const wchar_t*			wszText_;
    UINT32					cTextLength_;
};

