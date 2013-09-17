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
	rotation = 0.0f;

}

void OpenGL::updateScene(float p_dt)
{
	GLApp::updateScene(p_dt);
	rotation += p_dt;
}

void OpenGL::drawScene()
{
	GLApp::drawScene();
	float ratio;
	int width, height;
	glfwGetFramebufferSize(m_hMainWnd, &width, &height);
	ratio = width / (float) height;
	glViewport(0, 0, width, height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-ratio, ratio, -1.f, 1.f, 1.f, -1.f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glRotatef((float) rotation * 50.f, 0.f, 0.f, 1.f);
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
