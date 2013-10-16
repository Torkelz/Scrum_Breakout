#pragma once

#include "d3dUtil.h"
#include <d2d1.h>
#include <dwrite.h>

class AdvancedText
{
public:
	AdvancedText();
    ~AdvancedText();

    HRESULT Initialize(HWND hwndParent);

    HWND GetHwnd() { return hwnd_; }

private:
    HRESULT CreateDeviceIndependentResources(
        );

    void DiscardDeviceIndependentResources(
        );

    HRESULT CreateDeviceResources(
        );

    void DiscardDeviceResources(
        );

    HRESULT DrawD2DContent(
        );

    HRESULT DrawText(
        );

    void OnResize(
        UINT width,
        UINT height
        );

    static LRESULT CALLBACK WndProc(
        HWND hWnd,
        UINT message,
        WPARAM wParam,
        LPARAM lParam
        );

private:
    HWND hwnd_;

    // how much to scale a design that assumes 96-DPI pixels
    float dpiScaleX_;
    float dpiScaleY_;

    // Direct2D

    ID2D1Factory* pD2DFactory_;
    ID2D1HwndRenderTarget* pRT_;
    ID2D1SolidColorBrush* pBlackBrush_;

    // DirectWrite

    IDWriteFactory* pDWriteFactory_;
    IDWriteTextFormat* pTextFormat_;

    const wchar_t* wszText_;
    UINT32 cTextLength_;
};

