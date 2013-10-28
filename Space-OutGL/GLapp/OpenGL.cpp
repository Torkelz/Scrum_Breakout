#include "Header/OpenGL.h"
#include "glm/gtc/matrix_transform.hpp"
#include "Header/Ball.h"
#include "Header/Pad.h"

int main(void)
{
	OpenGL theApp;
	theApp.initApp();
    return theApp.run();
}

OpenGL::OpenGL() : GLApp()
{
	m_pCamera = 0;
	m_keyValue = 0x00;
	m_pHID = 0;
	m_ratio = 0;
	m_rotation = 0;
	m_pTexBlock = NULL;
	m_pTexBall = NULL;
	m_pBlockPic = 0;
	m_pBallPic = 0;
	m_scale = 1.f;
	m_game = NULL;

	m_pTexPad = NULL;
	m_pPadPic = 0;
}

OpenGL::~OpenGL()
{
	m_blockBuffer[0].~Buffer();
	m_blockBuffer[1].~Buffer();
	m_blockBuffer[2].~Buffer();
	m_blockBuffer[3].~Buffer();
	m_uniBlockBuffer.~Buffer();
	m_triShader.~Shader();

	m_ballBuffer.~Buffer();
	m_uniBallbuffer.~Buffer();
	m_ballShader.~Shader();
}

void OpenGL::initApp()
{
	GLApp::initApp();
	m_pTexBlock->init();


	//Uniform performs a padding on the buffer that can make
	//the result look really odd. Always use vec4 or mat4x4

	m_pTexBlock->createTexture("./GLapp/Picatures/block.png", m_pBlockPic);
	m_scale = 0;
	m_game = Game(m_hMainWnd);
	m_pPUObserver                = new PUObserver(this);
	m_game.init(m_pPUObserver, NORMAL);
	m_pCamera = m_game.getCamera();

	bool returnValue;
	//##BLOCKSTART##//
	for(int i = 0; i < 4; i++)
	{
		BufferInputDesc* blockBufferDesc = new BufferInputDesc[1];
		blockBufferDesc->size		= 3;
		blockBufferDesc->type		= GL_FLOAT;
		blockBufferDesc->normalized	= GL_FALSE;
		blockBufferDesc->pointer	= sizeof(vec3);

		returnValue = m_blockBuffer[i].init(GL_ARRAY_BUFFER, m_game.getField(i)->getBufferData(), sizeof(BlockVertex), m_game.getField(i)->getBlockListSize(), GL_STATIC_DRAW, blockBufferDesc, 1);
		if(!returnValue)
		{
			glfwSetWindowShouldClose(m_hMainWnd,1);
		}
	}

	returnValue =	m_uniBlockBuffer.init(GL_UNIFORM_BUFFER, m_uniBlock, sizeof(SuniBlock), 1, GL_DYNAMIC_DRAW, NULL, 0);
	if(!returnValue)
		glfwSetWindowShouldClose(m_hMainWnd, GL_TRUE);

	GLuint bindingPoint = 0; // Different data to be sent need to be different bindingPoint

	m_triShader.init();

	returnValue = m_triShader.createAndCompile(VERTEX_SHADER, "GLUtility/GLSL/BlockVertex.glsl");
	if(!returnValue)
				glfwSetWindowShouldClose(m_hMainWnd, GL_TRUE);

	returnValue = m_triShader.createAndCompile(GEOMETRY_SHADER, "GLUtility/GLSL/BlockGeometry.glsl");
	if(!returnValue)
				glfwSetWindowShouldClose(m_hMainWnd, GL_TRUE);

	returnValue = m_triShader.createAndCompile(FRAGMENT_SHADER, "GLUtility/GLSL/BlockFragment.glsl");
	if(!returnValue)
				glfwSetWindowShouldClose(m_hMainWnd, GL_TRUE);
	m_triShader.attachAndLink();


	// Fetch bindingPoint for uniform buffering.
	m_triShader.uniformBlockBinding(bindingPoint, "uniBlock");
	// Bind the bindingPoint to the right buffer.
	m_uniBlockBuffer.bindBufferBase(bindingPoint);
	bindingPoint++;
	// ##BLOCK END ## //

	// ##PAD START## //
	m_pTexPad->init();
	m_pTexPad->createTexture("./GLapp/Picatures/PadTexture.png", m_pPadPic);


	unsigned int const nrVertices = 4;
	padVertex data[nrVertices];
	std::vector<vec3>* t_positions = m_game.getPad()->getVertices();

	for(unsigned int i = 0; i < nrVertices; i++)
	{
		data[i].m_position = t_positions->at(i);
	}

	data[0].m_texCoord = vec2(1.0f, 1.0f);
	data[1].m_texCoord = vec2(0.0f, 1.0f);
	data[2].m_texCoord = vec2(1.0f, 0.0f);
	data[3].m_texCoord = vec2(0.0f, 0.0f);

	BufferInputDesc* padBufferDesc = new BufferInputDesc[2];
	padBufferDesc[0].size = 3;
	padBufferDesc[0].type = GL_FLOAT;
	padBufferDesc[0].normalized = GL_FALSE;
	padBufferDesc[0].pointer = sizeof(vec3);
	padBufferDesc[1].size = 2;
	padBufferDesc[1].type = GL_FLOAT;
	padBufferDesc[1].normalized = GL_FALSE;
	padBufferDesc[1].pointer = sizeof(vec2);

	returnValue = m_padBuffer.init(GL_ARRAY_BUFFER, &data, sizeof(padVertex), 4, GL_STATIC_DRAW, padBufferDesc, 2);

	returnValue = m_uniPadbuffer.init(GL_UNIFORM_BUFFER, &m_uniPad, sizeof(SuniPad), 1, GL_STATIC_DRAW, NULL, 0);

	m_padShader.init();
	returnValue = m_padShader.createAndCompile(VERTEX_SHADER, "GLUtility/GLSL/PadVertex.glsl");
		if(!returnValue)
						glfwSetWindowShouldClose(m_hMainWnd, GL_TRUE);
	returnValue = m_padShader.createAndCompile(FRAGMENT_SHADER, "GLUtility/GLSL/PadFragment.glsl");
		if(!returnValue)
						glfwSetWindowShouldClose(m_hMainWnd, GL_TRUE);
	m_padShader.attachAndLink();

	m_padShader.uniformBlockBinding(bindingPoint,"uniPad");
	m_uniPadbuffer.bindBufferBase(bindingPoint);
	bindingPoint++;
	// ##PAD END## //


	// ##BALL START## //
	m_pTexBall->init();
	m_pTexBall->createTexture("./GLapp/Picatures/BallTexture.png", m_pBallPic);

	vec3 tempVec = vec3(0.f,0.f,0.f);
	BufferInputDesc* ballBufferDesc = new BufferInputDesc[1];
	ballBufferDesc->size = 3;
	ballBufferDesc->type = GL_FLOAT;
	ballBufferDesc->normalized = GL_FALSE;
	ballBufferDesc->pointer = sizeof(vec3);

	returnValue = m_ballBuffer.init(GL_ARRAY_BUFFER, &tempVec, sizeof(vec3), 1, GL_STATIC_DRAW, ballBufferDesc, 1);

	returnValue =	m_uniBallbuffer.init(GL_UNIFORM_BUFFER, m_uniBall, sizeof(SuniBall), 1, GL_DYNAMIC_DRAW, NULL, 0);

	m_ballShader.init();
	returnValue = m_ballShader.createAndCompile(VERTEX_SHADER, "GLUtility/GLSL/BallVertex.glsl");
	if(!returnValue)
					glfwSetWindowShouldClose(m_hMainWnd, GL_TRUE);
	returnValue = m_ballShader.createAndCompile(GEOMETRY_SHADER, "GLUtility/GLSL/BallGeometry.glsl");
	if(!returnValue)
					glfwSetWindowShouldClose(m_hMainWnd, GL_TRUE);
	returnValue = m_ballShader.createAndCompile(FRAGMENT_SHADER, "GLUtility/GLSL/BallFragment.glsl");
	if(!returnValue)
					glfwSetWindowShouldClose(m_hMainWnd, GL_TRUE);
	m_ballShader.attachAndLink();

	m_ballShader.uniformBlockBinding(bindingPoint, "uniBall");
	m_uniBallbuffer.bindBufferBase(bindingPoint);
	bindingPoint++;
	// ##BALL END## //

	m_pHID = new HID(m_hMainWnd);

	for (int i = 0; i  < 4; i ++)
	{
		m_game.getField(i)->transBorders(i % 2);
	}
//	glfwSetCursorPos(m_hMainWnd, 1024/2, 768/2);
//	GLint texUnitLoc = glGetUniformLocation(m_triShader.getProgram(),(char*)("textest"));



	m_pHID->getObservable()->addSubscriber(m_game.getObserver());
}

void OpenGL::updateScene(float p_dt)
{
	GLApp::updateScene(p_dt);
	m_game.update(800, p_dt);
	int width, height;
	glfwGetFramebufferSize(m_hMainWnd, &width, &height);
	m_ratio = width / (float) height;
	glViewport(0, 0, width, height);
	m_rotation += p_dt;
	updateFPSCounter();
	generatingKeyValue();

	unsigned int active = m_game.getActiveFieldNr();
	if(m_game.getField(active)->getUpdateBuffer())
	{
		m_blockBuffer[active].apply();
		int u = sizeof(BlockVertex);
		glBufferSubData(GL_ARRAY_BUFFER, 0, u*m_game.getActiveField()->getBlockListSize(), m_game.getActiveField()->getBufferData());
		m_blockBuffer[active].deApply();

		m_game.getField(active)->setUpdateBuffer(false);
	}
}

void OpenGL::drawScene()
{
	GLApp::drawScene();
	mat4 playFieldRotation = m_game.getActiveField()->getRotationMatrix();


	m_scale += 0.01f;
	//Box Update #################
	m_uniBlock[0].proj = m_pCamera->getProjectionMatrix() * m_pCamera->getViewMatrix();
	m_pTexBlock->bindTextureResource(m_triShader.getProgram(), (char*)("texBlock"), m_pBlockPic);
	//Update buffer here!

	m_uniBlockBuffer.setSubData(0, sizeof(m_uniBlock), m_uniBlock);
	m_triShader.apply();
//	// stop updating here!
	glDisable(GL_CULL_FACE);
	for(int i = 0; i < 4; i++)
	{
		m_blockBuffer[i].apply();
	// Draw the triangle !
		glDrawArrays(GL_POINTS, 0, m_game.getField(i)->getBlockListSize()); // Starting from vertex 0; 3 vertices total -> 1 triangle
		m_blockBuffer[i].deApply();
	}
	m_uniBlockBuffer.deApply();
	//##############################

	//## BALL ##
	m_pTexBall->bindTextureResource(m_ballShader.getProgram(), (char*)("texBall"), m_pBallPic);
	m_ballShader.apply();
	vec3 t_ballPos = ((Ball*)m_game.getBall(0))->getRealPosition();
	m_uniBall[0].Trans = glm::translate(t_ballPos.x, t_ballPos.y, t_ballPos.z);
	m_uniBall[0].projView = m_pCamera->getProjectionMatrix() * m_pCamera->getViewMatrix();
	m_uniBall[0].eyepos = vec4(m_pCamera->getPosition(),1.0f);
	m_uniBall[0].size = vec4(5.0f,5.0f,0,0);

	m_uniBallbuffer.setSubData(0,sizeof(m_uniBall), m_uniBall);

	m_ballBuffer.apply();
	// Draw the triangle !
	glDrawArrays(GL_POINTS, 0, 1); // Starting from vertex 0; 3 vertices total -> 1 triangle
	m_ballBuffer.deApply();
	m_uniBallbuffer.deApply();
	//#############################

	//## PAD ##
	m_pTexPad->bindTextureResource(m_padShader.getProgram(), (char*)("texPad"), m_pPadPic);
	m_padShader.apply();
	mat4 translatePadMatrix;

	vec3 padPos = ((Pad*)(m_game.getPad()))->getRealPosition();
	translatePadMatrix = glm::translate(padPos.x, padPos.y, padPos.z);

	mat4 t_scaleMatrix = mat4(1.0f) * ((Pad*)(m_game.getPad()))->getScale();
	t_scaleMatrix[3][3] = 1.0f;
	mat4 PVW =  m_pCamera->getProjectionMatrix() * m_pCamera->getViewMatrix() * translatePadMatrix * playFieldRotation * t_scaleMatrix;
	m_uniPad[0].PVW = PVW;
	m_uniPadbuffer.setSubData(0,sizeof(m_uniPad), m_uniPad);
	m_padBuffer.apply();

	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	m_padBuffer.apply();
	m_uniPadbuffer.deApply();

	//#############################

}

void  OpenGL::messageCallback(GLFWwindow* p_pMainWnd, int p_key, int p_scanCode, int p_action, int p_mods)
{
	if(p_action == GLFW_PRESS || p_action == GLFW_REPEAT){
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
	case GLFW_KEY_W:
		m_keyValue = GLFW_KEY_W;
		break;
	case GLFW_KEY_S:
		m_keyValue = GLFW_KEY_S;
		break;
	case GLFW_KEY_Q:
			m_keyValue = GLFW_KEY_Q;
			break;
	case GLFW_KEY_E:
			m_keyValue = GLFW_KEY_E;
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
void OpenGL::addPowerUp(PowerUp* p_pPowerUp)
{
        m_powerUps.push_back(p_pPowerUp);
        //Update buffer
}

void OpenGL::removePowerUp(int i)
{
        m_powerUps.erase(m_powerUps.begin() + i);
}
