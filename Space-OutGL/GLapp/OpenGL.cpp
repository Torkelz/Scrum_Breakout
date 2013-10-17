#include "Header/OpenGL.h"
#include "glm/gtc/matrix_transform.hpp"
//struct triangleVertex
//{
//	vec3 pos;
//	vec3 col;
//};
//triangleVertex tri[1];

struct vertexColor
{
	vec4 pos;
	vec4 tex;
	mat4 proj;
	//0.78f, 0.651f, 0.8f
};
vertexColor col[1];
float colValue;

int main(void)
{
	OpenGL theApp;
	theApp.initApp();
    return theApp.run();
}

OpenGL::OpenGL() : GLApp()
{
	m_keyValue = 0x00;
	m_pHID = 0;
	m_ratio = 0;
	m_rotation = 0;
	m_pTexTest = NULL;
	m_pTexture = 0;
	m_scale = 1.f;
}

OpenGL::~OpenGL()
{
	triBuffer.~Buffer();
	m_uniBuffer.~Buffer();
	m_triShader.~Shader();
}

void OpenGL::initApp()
{
	GLApp::initApp();
	m_pTexTest->init();
	col[0].pos = vec4(0.0f, 0.0f, 0.f,1.0f); //Uniform performs a padding on the buffer that can make
	col[0].tex = vec4(0.f, 0.f, 0.f, 0.f);   //the result look really odd. Always use vec4 or mat4x4;
	m_pTexTest->createTexture("./GLapp/Textures/cat.png", m_pTexture);
	m_scale = 0;
//	BufferInputDesc* desc = new BufferInputDesc[2];
//	desc[0].size = 3;
//	desc[0].type = GL_FLOAT;
//	desc[0].normalized = GL_FALSE;
//	desc[0].pointer = sizeof(vec3);
//	desc[1].size = 3;
//	desc[1].type = GL_FLOAT;
//	desc[1].normalized = GL_FALSE;
//	desc[1].pointer = sizeof(vec3);
//
	bool returnValue;// =	triBuffer.init(GL_ARRAY_BUFFER, tri, sizeof(triangleVertex),1, GL_STATIC_DRAW, desc, 2);
//	if(!returnValue)
//		glfwSetWindowShouldClose(m_hMainWnd, GL_TRUE);

	returnValue =	m_uniBuffer.init(GL_UNIFORM_BUFFER, col, sizeof(vertexColor), 1, GL_DYNAMIC_DRAW, NULL, 0);
	if(!returnValue)
		glfwSetWindowShouldClose(m_hMainWnd, GL_TRUE);

	GLuint bindingPoint = 0; // Different data to be sent need to be different bindingPoint

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


	// Fetch bindingPoint for uniform buffering.
	m_triShader.uniformBlockBinding(bindingPoint, "ColorBlock");
	// Bind the bindingPoint to the right buffer.
	m_uniBuffer.bindBufferBase(bindingPoint);

	//m_pGame = new Game();
	//m_pGame->init();

	m_pHID = new HID(m_hMainWnd);

//	glfwSetCursorPos(m_hMainWnd, 1024/2, 768/2);
//	GLint texUnitLoc = glGetUniformLocation(m_triShader.getProgram(),(char*)("textest"));
	m_pTexTest->bindTextureResource(m_triShader.getProgram(), (char*)("texUnit"), m_pTexture);
	//m_pHID->getObservable()->addSubscriber(m_pGame->getObserver());
}

void OpenGL::updateScene(float p_dt)
{
	GLApp::updateScene(p_dt);
	generatingKeyValue();
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

	m_triShader.apply();

	m_scale += 0.01f;

	mat4 view;

	vec4 target = vec4(0.0f, 0.0f, 1.0f, 1.0f);
	vec4 up = vec4(0.0f, 1.0f, 0.0f, 1.0f);

    vec4 N = target;
    N = normalize(N);

    vec4 U = up;
    U = normalize(U);

    vec3 R = glm::cross(vec3(U.x, U.y, U.z), vec3(target.x, target.y, target.z));

    view[0][0] = U.x; view[0][1] = U.y; view[0][2] = U.z; view[0][3] = 0.0f;
    view[1][0] = R.x; view[1][1] = R.y; view[1][2] = R.z; view[1][3] = 0.0f;
    view[2][0] = N.x; view[2][1] = N.y; view[2][2] = N.z; view[2][3] = 0.0f;
    view[3][0] = 0.0f; view[3][1] = 0.0f; view[3][2] = -2.0f; view[3][3] = 1.0f;

	mat4 proj;

//    const float ar         = m_ClientWidth / m_ClientHeight;
//    const float zNear      = 1.0f;
//    const float zFar       = 100.0f;
//    const float zRange     = zNear - zFar;
//    const float tanHalfFOV = tanf((30.0f / 2.0f)*M_PI/180.0f);


    mat4 World;

    World[0][0] = 1.0f; World[0][1] = 0.0f; World[0][2] = 0.0f; World[0][3] = 0.0f;
    World[1][0] = 0.0f; World[1][1] = 1.0f; World[1][2] = 0.0f; World[1][3] = 0.0f;
    World[2][0] = 0.0f; World[2][1] = 0.0f; World[2][2] = 1.0f; World[2][3] = 0.0f;
    World[3][0] = 0.0f; World[3][1] = 0.0f; World[3][2] = 0.0f; World[3][3] = 1.0f;

	mat4 ry;
	mat4 rx;

	    ry[0][0] = cosf(m_scale);	ry[0][1] = 0.0f; 	ry[0][2] = -sinf(m_scale); ry[0][3] = 0.0f;
	    ry[1][0] = 0.0f;			ry[1][1] = 1.0f;  	ry[1][2] = 0.0f; 		ry[1][3] = 0.0f;
	    ry[2][0] = sinf(m_scale);	ry[2][1] = 0.0f;     ry[2][2] = cosf(m_scale); 	ry[2][3] = 0.0f;
	    ry[3][0] = 0.0f;        	ry[3][1] = 0.0f;     ry[3][2] = 0.0f; 		ry[3][3] = 1.0f;

	    rx[0][0] = 1.0f; rx[0][1] = 0.0f   ; rx[0][2] = 0.0f    ; rx[0][3] = 0.0f;
	    rx[1][0] = 0.0f; rx[1][1] = cosf(m_scale); rx[1][2] = -sinf(m_scale); rx[1][3] = 0.0f;
	    rx[2][0] = 0.0f; rx[2][1] = sinf(m_scale); rx[2][2] = cosf(m_scale) ; rx[2][3] = 0.0f;
	    rx[3][0] = 0.0f; rx[3][1] = 0.0f   ; rx[3][2] = 0.0f    ; rx[3][3] = 1.0f;
	    proj = glm::perspective(45.f, 1.f, 1.0f, 100.f);
	    col[0].proj = proj * view * glm::transpose(ry * rx);
	    //glUniformMatrix4fv(glGetUniformLocation(m_triShader.getProgram(), "gWorld"), 1, GL_TRUE, &World.m[0][0]);
	//Update buffer here!

	m_uniBuffer.setSubData(0, sizeof(col), col);

	// stop updating here!
	glDisable(GL_CULL_FACE);
	triBuffer.apply();
	// Draw the triangle !
	glDrawArrays(GL_POINTS, 0, 1); // Starting from vertex 0; 3 vertices total -> 1 triangle
	m_uniBuffer.deApply();
	triBuffer.deApply();

}

void  OpenGL::messageCallback(GLFWwindow* p_pMainWnd, int p_key, int p_scanCode, int p_action, int p_mods)
{
	if(p_action == GLFW_PRESS){
		keyValueStatic = p_key;

	}
	else
		keyValueStatic = 0x00;
}

//void OpenGL::mousePosCallback(GLFWwindow* p_pMainWnd, double p_mouse_x, double p_mouse_y)
//{
//	mouseStatic_x = p_mouse_x;
//	mouseStatic_y = p_mouse_y;
//}

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

void OpenGL::generatingKeyValue()
{
	switch(keyValueStatic)
	{
	case GLFW_KEY_A:
		m_keyValue = GLFW_KEY_A;
		break;
	case GLFW_KEY_D:
		m_keyValue = GLFW_KEY_D;
		break;
	case GLFW_KEY_SPACE:
		m_keyValue = GLFW_KEY_SPACE;
		break;
	case GLFW_KEY_LEFT:
		m_keyValue = GLFW_KEY_LEFT;
		break;
	case GLFW_KEY_RIGHT:
		m_keyValue = GLFW_KEY_RIGHT;
		break;
	case GLFW_KEY_ESCAPE:
		m_keyValue = GLFW_KEY_ESCAPE;
		break;
	default:
		m_keyValue = 0x00;
		break;
	}
	m_pHID->update(m_keyValue);
}
