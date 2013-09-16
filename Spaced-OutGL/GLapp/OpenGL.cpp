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
}

void OpenGL::onResize()
{
	GLApp::onResize();
}

void OpenGL::updateScene(float p_dt)
{
	GLApp::updateScene(p_dt);
}

void OpenGL::drawScene()
{
	GLApp::drawScene();
}
