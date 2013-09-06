
/*

#include "Global.h"
#include "Graphics.h"

#ifdef _WIN32
#include <windows.h>
#include "d3dUtil.h"
#include "DirectX11.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance,
				   PSTR cmdLine, int showCmd)
{
	// Enable run-time memory check for debug builds.
#if defined(DEBUG) | defined(_DEBUG)
	_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
#endif

	Global::isDirectX = true;

	Graphics* m_pGraphicsDevice = new DirectX11(hInstance);

	m_pGraphicsDevice->initApplication();

	return m_pGraphicsDevice->run();
}
#elif __linux__ 
int main() //linux main
{
	Global::isDirectX = false;

	return 0;
}
#else
int main()
{
	return 0;
}
#endif

*/