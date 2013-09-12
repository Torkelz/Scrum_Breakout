#ifndef D3DUTIL_H
#define D3DUTIL_H

// Let VC++ know we are compiling for WinXP and up.  This let's us use
// API functions specific to WinXP (e.g., WM_INPUT API).
#ifndef _WIN32_WINNT
#define _WIN32_WINNT   0x0600 // Vista
#endif

 
// Enable extra D3D debugging in debug builds if using the debug DirectX runtime.  
// This makes D3D objects work well in the debugger watch window, but slows down 
// performance slightly.
#if defined(DEBUG) || defined(_DEBUG)
	#ifndef D3D_DEBUG_INFO
	#define D3D_DEBUG_INFO
	#endif
#endif

#if defined(DEBUG) || defined(_DEBUG)
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#endif

#include <d3d11.h>
#include <cassert>
#include <vector>
#include <D3D11Shader.h>
#include <DirectXMath.h>
#include <d3dCompiler.h>


//*****************************************************************************
// Convenience macro for releasing COM objects.
//*****************************************************************************

#define ReleaseCOM(x) { if(x){ x->Release();x = 0; } }

//*****************************************************************************
// Constants
//*****************************************************************************

const float INFINITY = FLT_MAX;
const float PI       = 3.14159265358979323f;
const float TWOPI	 = 2 * PI;
const float MATH_EPS = 0.0001f;

#endif // D3DUTIL_H