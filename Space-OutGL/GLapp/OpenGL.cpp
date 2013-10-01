#include "Header/OpenGL.h"


struct triangleVertex
{
	vertexVec pos;
	vertexVec col;
};
triangleVertex tri[1];

int main(void)
{
	OpenGL theApp;
	theApp.initApp();
    return theApp.run();
}

OpenGL::OpenGL() : GLApp()
{
	m_ratio = 0;
	m_rotation = 0;
}

OpenGL::~OpenGL()
{
	triBuffer.~Buffer();
	m_triShader.~Shader();
}

void OpenGL::initApp()
{
	GLApp::initApp();
	m_rotation = 0.0f;



	tri[0].pos = vertexVec(-1.0f, -1.0f, 0.f);
	tri[0].col = vertexVec(1.0f,0.0f,0.0f);
//	tri[1].pos = vertexVec(1.0f, -1.0f, 0.f);
//	tri[1].col = vertexVec(0.0f,1.0f,0.0f);
//	tri[2].pos = vertexVec(0.f, 1.0f, 0.f);
//	tri[2].col = vertexVec(0.0f,0.0f,1.0f);

	BufferInputDesc* desc = new BufferInputDesc[1];
	desc[0].size = 3;
	desc[0].type = GL_FLOAT;
	desc[0].normalized = GL_FALSE;
	desc[0].pointer = sizeof(vec3);
	desc[1].size = 3;
	desc[1].type = GL_FLOAT;
	desc[1].normalized = GL_FALSE;
	desc[1].pointer = sizeof(vec3);

	bool returnValue =	triBuffer.init(GL_ARRAY_BUFFER, tri, sizeof(triangleVertex),3, GL_STATIC_DRAW, desc, 2);
	if(!returnValue)
		glfwSetWindowShouldClose(m_hMainWnd, GL_TRUE);

	m_triShader.init();

	returnValue = m_triShader.createAndCompile(VERTEX_SHADER, "GLUtility/GLSL/triVertex.glsl");
	if(!returnValue)
				glfwSetWindowShouldClose(m_hMainWnd, GL_TRUE);

	returnValue = m_triShader.createAndCompile(GEOMETRY_SHADER, "GLUtility/GLSL/triGeometry.glsl");
	if(!returnValue)
				glfwSetWindowShouldClose(m_hMainWnd, GL_TRUE);

	returnValue = m_triShader.createAndCompile(FRAGMENT_SHADER, "GLUtility/GLSL/triFragment.glsl");
	if(!returnValue)
				glfwSetWindowShouldClose(m_hMainWnd, GL_TRUE);
	m_triShader.attachAndLink();
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
	//glUseProgram(m_program);
	m_triShader.apply();
	triBuffer.apply();
	// Draw the triangle !
	glDrawArrays(GL_POINTS, 0, 1); // Starting from vertex 0; 3 vertices total -> 1 triangle
	triBuffer.deApply();

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
