#ifndef GRAPHICS_H
#define GRAPHICS_H
#include <string>

class Graphics
{
public:
	Graphics()
	{
		m_AppPaused = false;
		m_Minimized = false;
		m_Maximized = false;
		m_Resizing  = false;
		m_ClientHeight = 600;
		m_ClientWidth = 800;
		m_MainWindowTitle = L"Space-out";
	};
	virtual void initApplication() = 0;
	virtual int run() = 0;
	virtual void getAppInst() = 0; //Placeholder, check type for openGL/DX11
	virtual void getMainWindow() = 0; //Placeholder, check type for openGL/DX11
	virtual void onResize() = 0;
	virtual void updateScene() = 0;
	virtual void drawScene() = 0;
	
	
protected:
	virtual void initMainWindow() = 0;
	virtual void initGraphics() = 0;
	bool m_AppPaused;
	bool m_Minimized;
	bool m_Maximized;
	bool m_Resizing;
	unsigned int m_ClientWidth;
	unsigned int m_ClientHeight;
	std::wstring m_MainWindowTitle;

};

#endif