#include "Header/GLApp.h"

GLApp::GLApp()
{
	m_hMainWnd   = 0;
	m_AppPaused  = false;
	m_Minimized  = false;
	m_Maximized  = false;
	m_Resizing   = false;

	m_MainWndTitle = "Space-out";
	m_ClientWidth    = 1024;
	m_ClientHeight   = 768;

}
GLApp::~GLApp()
{
	glfwTerminate();
}

void GLApp::initApp()
{
	initMainWindow();
	initOpenGL();
}

int GLApp::run()
{
	while (!glfwWindowShouldClose(m_hMainWnd))
	{
		/* Render here */

		/* Swap front and back buffers */
		glfwSwapBuffers(m_hMainWnd);

		/* Poll for and process events */
		glfwPollEvents();
	}
	return 0;
}

void GLApp::onResize()
{

}

void GLApp::updateScene(float p_dt)
{
	p_dt = 0;
}

void GLApp::drawScene()
{

}

void GLApp::initMainWindow()
{
	if (!glfwInit())
		glfwTerminate();
}

void GLApp::initOpenGL()
{
	m_hMainWnd = glfwCreateWindow(m_ClientWidth, m_ClientHeight, m_MainWndTitle.c_str(), NULL, NULL);
	    if (!m_hMainWnd)
	    {
	        glfwTerminate();
	    }
}
