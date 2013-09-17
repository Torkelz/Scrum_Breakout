#include "Header/GLApp.h"

#include "Header/OpenGL.h"

GLApp::GLApp()
{
	m_hMainWnd   = 0;
	m_appPaused  = false;
	m_minimized  = false;
	m_maximized  = false;
	m_resizing   = false;

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
	glfwMakeContextCurrent(m_hMainWnd);

	glfwSetKeyCallback(m_hMainWnd, OpenGL::messageCallback);

	//glfwSetWindowSizeCallback(m_hMainWnd, resizeCallback);

	double currTime = glfwGetTime();
	int tWidth, tHeight;

	while (!glfwWindowShouldClose(m_hMainWnd))
	{

		//Check if window is resized
		glfwGetWindowSize(m_hMainWnd, &tWidth, &tHeight);
		if(tWidth != m_ClientWidth || tHeight != m_ClientHeight)
			m_appPaused = true;
		/* Render here */
		if( !m_appPaused )
		{
			//Calc dt when application is not paused
			double newTime = glfwGetTime();
			double dt = newTime - currTime;
			currTime = newTime;
			std::ostringstream os;
			os << dt;
			std::string sdt = os.str();
			//Print debug dt
			glfwSetWindowTitle(m_hMainWnd, (m_MainWndTitle + "  " + sdt).c_str());


			//Update and Draw
			updateScene(dt);
			drawScene();



			glfwPollEvents();
		}
		else
		{
			glfwSetWindowTitle(m_hMainWnd, "PAUSED MOTHERFUKAAAAAAA");
			m_appPaused = false;
			glfwPollEvents();
			glfwGetWindowSize(m_hMainWnd, &tWidth, &tHeight);
						m_ClientWidth = tWidth;
						m_ClientHeight = tHeight;
			usleep(500000);

			//Keep dt paused
			currTime = glfwGetTime();
		}

		/* Poll for and process events */

	}
	return 0;
}

void GLApp::updateScene(float p_dt)
{
	p_dt = 0;
}

void GLApp::drawScene()
{
	/* Swap front and back buffers */
	glfwSwapBuffers(m_hMainWnd);
	glClear(GL_COLOR_BUFFER_BIT);
	glClearColor(0.0f, 0.0f, 1.0f, 0.5f);

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
