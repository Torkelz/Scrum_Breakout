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


	// Derived class should set these in derived constructor to customize starting values.
	string						m_MainWndTitle;
	int							m_ClientWidth;
	int							m_ClientHeight;

};
#endif /* GLAPP_H_ */
