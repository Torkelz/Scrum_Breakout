#include "Header/OpenGL.h"

int main(void)
{
	OpenGL theApp;
	theApp.initApp();
    return theApp.run();
}

OpenGL::OpenGL() : GLApp()
{

}

OpenGL::~OpenGL()
{

}

void OpenGL::initApp()
{
	GLApp::initApp();
	m_rotation = 0.0f;
}

void OpenGL::updateScene(float p_dt)
{
	GLApp::updateScene(p_dt);
	int width, height;
	glfwGetFramebufferSize(m_hMainWnd, &width, &height);
	m_ratio = width / (float) height;
	glViewport(0, 0, width, height);
	m_rotation += p_dt;
	updateFPSCounter();
}

void OpenGL::drawScene()
{
	GLApp::drawScene();

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-m_ratio, m_ratio, -1.f, 1.f, 1.f, -1.f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glRotatef((float) m_rotation * 50.f, 0.f, 0.f, 1.f);
	glBegin(GL_TRIANGLES);
	glColor3f(1.f, 0.f, 0.f);
	glVertex3f(-0.6f, -0.4f, 0.f);
	glColor3f(0.f, 1.f, 0.f);
	glVertex3f(0.6f, -0.4f, 0.f);
	glColor3f(0.f, 0.f, 1.f);
	glVertex3f(0.f, 0.6f, 0.f);
	glEnd();
}

void OpenGL::messageCallback(GLFWwindow* p_pMainWnd, int p_key, int p_scanCode, int p_action, int p_mods)
{
	if(p_key == GLFW_KEY_ESCAPE && p_action == GLFW_PRESS)
		{glfwSetWindowShouldClose(p_pMainWnd, GL_TRUE);}
}

void OpenGL::updateFPSCounter()
{
	 static double previous_seconds = glfwGetTime ();
	 static int frame_count;
	 double current_seconds = glfwGetTime ();
	 double elapsed_seconds = current_seconds - previous_seconds;
	 if (elapsed_seconds > 0.25)
	 {
		 previous_seconds = current_seconds;
		 double fps = (double)frame_count / elapsed_seconds;
		 char tmp[128];
	     sprintf (tmp, " @ fps: %.2lf", fps);
	     string strTemp = m_MainWndTitle;
	     strTemp.append(tmp);
	     glfwSetWindowTitle (m_hMainWnd, strTemp.c_str());
	     frame_count = 0;
	 }
	 frame_count++;
}
