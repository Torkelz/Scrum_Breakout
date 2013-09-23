#include "Header/OpenGL.h"
#include <fstream>


struct triangleVertex
{
	vec3 pos;
	vec3 col;
};
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
	m_program = 0;
}

OpenGL::~OpenGL()
{
	triBuffer.~Buffer();

}

void OpenGL::initApp()
{
	GLApp::initApp();
	m_rotation = 0.0f;
	triangleVertex tri[3];
	tri[0].pos = vec3(-0.6f, -0.4f, 0.f);
	tri[0].col = vec3(0.f, 0.f, 1.f);
	tri[1].pos = vec3(0.6f, -0.4f, 0.f);
	tri[1].col = vec3(0.f, 1.f, 0.f);
	tri[2].pos = vec3(0.f, 0.6f, 0.f);
	tri[2].col = vec3(1.f, 0.f, 0.f);

	BufferInputDesc desc[2];
	desc[0].size = 3;
	desc[0].type = GL_FLOAT;
	desc[0].normalized = GL_FALSE;
	desc[0].pointer = NULL;
	desc[1].size = 3;
	desc[1].type = GL_FLOAT;
	desc[1].normalized = GL_FALSE;
	desc[1].pointer = NULL;
	bool test =	triBuffer.init(GL_ARRAY_BUFFER, &tri, sizeof(triangleVertex),3, GL_STATIC_DRAW, desc, 2);
	if(!test)
		glfwSetWindowShouldClose(m_hMainWnd, GL_TRUE);
	GLuint fragment = glCreateShader(GL_FRAGMENT_SHADER);
	GLuint shader = glCreateShader(GL_VERTEX_SHADER);
		std::string VertexShaderCode;
		const char* vertexChar = "/home/bth/Desktop/GitHub/Git/Spaced-OutGL/GLapp/triVertex.vertexshader";
		std::ifstream VertexShaderStream(vertexChar);
		if(VertexShaderStream.is_open())
		{
			std::string Line = "";
			while(getline(VertexShaderStream, Line))
				VertexShaderCode += "\n" + Line;
			VertexShaderStream.close();
		}

		// Read the Fragment Shader code from the file
		std::string FragmentShaderCode;
		std::ifstream FragmentShaderStream("/home/bth/Desktop/GitHub/Git/Spaced-OutGL/GLapp/triFragment.fragmentshader", std::ios::in);
		if(FragmentShaderStream.is_open()){
			std::string Line = "";
			while(getline(FragmentShaderStream, Line))
				FragmentShaderCode += "\n" + Line;
			FragmentShaderStream.close();
		}
		const char* sourceVer = VertexShaderCode.c_str();
		GLint lengthVer= VertexShaderCode.length();

		glShaderSource(shader,1, &sourceVer, &lengthVer);
		const char* sourceFra = FragmentShaderCode.c_str();
		GLint lengthFra = FragmentShaderCode.length();
		glShaderSource(fragment,1,&sourceFra, &lengthFra);
		glCompileShader(shader);
		GLint status;
		    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
		    if (status == GL_FALSE)
		    {
		        GLint infoLogLength;
		        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLogLength);

		        GLchar *strInfoLog = new GLchar[infoLogLength + 1];
		        glGetShaderInfoLog(shader, infoLogLength, NULL, strInfoLog);

		        const char *strShaderType = NULL;
		        strShaderType = "vertex";


		        fprintf(stderr, "Compile failure in %s shader:\n%s\n", strShaderType, strInfoLog);
		        delete[] strInfoLog;
		    }

		glCompileShader(fragment);
		glGetShaderiv(fragment, GL_COMPILE_STATUS, &status);
			    if (status == GL_FALSE)
			    {
			        GLint infoLogLength;
			        glGetShaderiv(fragment, GL_INFO_LOG_LENGTH, &infoLogLength);

			        GLchar *strInfoLog = new GLchar[infoLogLength + 1];
			        glGetShaderInfoLog(fragment, infoLogLength, NULL, strInfoLog);

			        const char *strShaderType = NULL;
			        strShaderType = "fragment";


			        fprintf(stderr, "Compile failure in %s shader:\n%s\n", strShaderType, strInfoLog);
			        delete[] strInfoLog;
			    }
		m_program = glCreateProgram();
		glAttachShader(m_program, shader);
		glAttachShader(m_program, fragment);
		glLinkProgram(m_program);
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
	triBuffer.apply();

	glUseProgram(m_program);
	glDrawArrays(GL_TRIANGLES,0,triBuffer.getNumElem());
//	glBegin(GL_TRIANGLES);
//	glColor3f(1.f, 0.f, 0.f);
//	glVertex3f(-0.6f, -0.4f, 0.f);
//	glColor3f(0.f, 1.f, 0.f);
//	glVertex3f(0.6f, -0.4f, 0.f);
//	glColor3f(0.f, 0.f, 1.f);
//	glVertex3f(0.f, 0.6f, 0.f);
//	glEnd();
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
