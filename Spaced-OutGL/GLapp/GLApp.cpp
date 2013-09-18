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

	double currTime = glfwGetTime();
	int tWidth, tHeight;

	while (!glfwWindowShouldClose(m_hMainWnd))
	{
		//Check if window is resized
		glfwGetWindowSize(m_hMainWnd, &tWidth, &tHeight);
		if(tWidth != m_ClientWidth || tHeight != m_ClientHeight)
			m_appPaused = true;
		if( !m_appPaused )
		{
			//Calc dt when application is not paused
			double newTime = glfwGetTime();
			double dt = newTime - currTime;
			currTime = newTime;

			//Update and Draw
			updateScene(dt);
			drawScene();

			glfwPollEvents();
		}
		else
		{
			m_appPaused = false;
			glfwPollEvents();
			glfwGetWindowSize(m_hMainWnd, &tWidth, &tHeight);
			m_ClientWidth = tWidth;
			m_ClientHeight = tHeight;
			usleep(500000);

			//Keep dt paused
			currTime = glfwGetTime();
		}
	}
	return 0;
}

void GLApp::updateScene(float p_dt)
{

}

void GLApp::drawScene()
{
	/* Swap front and back buffers */
	glfwSwapBuffers(m_hMainWnd);
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0f, 0.0f, 1.0f, 0.5f);

}

void GLApp::initMainWindow()
{
	glfwSetErrorCallback(errorCallback);
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

void GLApp::errorCallback(int p_error, const char* p_pDescription)
{
	fputs(p_pDescription, stderr);
	gl_log(p_pDescription, __FILE__, __LINE__);
}

bool GLApp::gl_log(const char* p_pMessage, const char* p_pFilename, int p_line)
{
	FILE* file = fopen ("gl.log", "a+");
	  if (!file) {
	    fprintf (stderr, "ERROR: could not open %s for writing\n", "gl.log");
	    return false;
	  }
	  fprintf (file, "%s:%i %s\n", p_pFilename, p_line, p_pMessage);
	  fclose (file);
	  return true;
}
