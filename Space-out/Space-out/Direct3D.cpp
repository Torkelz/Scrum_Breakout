#include "Direct3D.h"
#include "Object.h"
#include "ABlock.h"
#include "Ball.h"
#include "Pad.h"



int WINAPI WinMain(HINSTANCE p_hInstance, HINSTANCE p_prevInstance,
				   PSTR p_cmdLine, int p_showCmd)
{
	// Enable run-time memory check for debug builds.
#if defined(DEBUG) | defined(_DEBUG)
	_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
#endif

	Direct3D theApp(p_hInstance);
	
	theApp.initApp();

	return theApp.run();
}

Direct3D::Direct3D(HINSTANCE p_hInstance)
: D3DApp(p_hInstance) {}

Direct3D::~Direct3D()
{
	ReleaseCOM(m_pDevice);
	ReleaseCOM(m_pDeviceContext);
	ReleaseCOM(m_pSwapChain);
	ReleaseCOM(m_pDepthStencilBuffer);
	ReleaseCOM(m_pRenderTargetView);
	ReleaseCOM(m_pDepthStencilView);
}

void Direct3D::initApp()
{
	D3DApp::initApp();
	HRESULT hr = S_OK;
	m_buffer = Buffer();
	m_cBuffer = Buffer();
	m_shader = Shader();

	for(int i = 0; i < 4; i++)
		m_blockBuffers[i]	= Buffer();

	m_blockShader		= Shader();
	m_cBlockBuffer		= Buffer();
	m_pPUObserver		= new PUObserver(this);
	m_game = Game();
	m_game.init(m_pPUObserver);


	//Set up world view proj
	//m_camPosition = XMVectorSet( 0.0f, 0.0f, 250.f, 0.0f );
	//m_camTarget = XMVectorSet( 0.0f, 0.0f, 0.0f, 0.0f );
	//m_camUp = XMVectorSet( 0.0f, 1.0f, 0.0f, 0.0f );
	//
	//m_camView = XMMatrixLookAtLH( m_camPosition, m_camTarget, m_camUp );
	//m_camProjection = XMMatrixPerspectiveFovLH( 0.4f*3.14f, (float)m_ClientWidth/m_ClientHeight, 1.0f, 1000.0f);
	//m_camProjection = XMMatrixPerspectiveFovLH( PI*0.25f, (float)m_ClientWidth/m_ClientHeight, 1.0f, 500.0f);

	m_camera = new Camera(vec3(0.0f, 0.0f, -250.0f));
	m_camera->setViewMatrix(vec3(0.0f, 0.0f, -250.0f));
	m_camera->createProjectionMatrix(PI*0.25f,(float)m_ClientWidth/m_ClientHeight, 1.0f, 500.0f);
	m_camView = mat4ToXMMatrix(m_camera->getViewMatrix());
	m_camProjection = mat4ToXMMatrix(m_camera->getProjectionMatrix());
	m_camPosition = vec3ToXMVector(m_camera->getPosition());
	m_world = XMMatrixIdentity();
	
	RECT r;
	GetClientRect(m_hMainWnd, &r);
	vec2 playFieldScreen;
	playFieldScreen.x = (m_game.getActiveField()->getScreenPosition(XMMatrixTomat4(&(m_camView*m_camProjection))).x + 1)/2 * r.right;
	((Pad*)(m_game.getPad()))->setMouseOffset(m_game.getActiveField()->getSize().x / r.right);

	// ## PAD ##
	UINT32 const nrVertices = 4;
	Vertex data[nrVertices];
	std::vector<vec3>* t_positions = m_game.getPad()->getVertices();

	for(int i = 0; i < nrVertices; i++)
	{
		data[i].m_position = t_positions->at(i);
	}

	data[0].m_textureCoordinates = vec2(1.0f, 0.0f);
	data[1].m_textureCoordinates = vec2(1.0f, 1.0f);
	data[2].m_textureCoordinates = vec2(0.0f, 0.0f);
	data[3].m_textureCoordinates = vec2(0.0f, 1.0f);

	BufferInitDesc bufferDesc;
	bufferDesc.elementSize = sizeof(Vertex);
	bufferDesc.initData = &data;
	bufferDesc.numElements = nrVertices;
	bufferDesc.type = VERTEX_BUFFER;
	bufferDesc.usage = BUFFER_USAGE_IMMUTABLE;
	
	m_buffer.init(m_pDevice, m_pDeviceContext, bufferDesc);
	
	D3D11_INPUT_ELEMENT_DESC desc[] = 
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TEXTUREPOSITION", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};
	
	m_shader.init(m_pDevice, m_pDeviceContext, 2);
	m_shader.compileAndCreateShaderFromFile(L"VertexShader.fx", "main","vs_5_0", VERTEX_SHADER , desc);
	m_shader.compileAndCreateShaderFromFile(L"PixelShader.fx", "main", "ps_5_0", PIXEL_SHADER, NULL);
	
	m_WVP	= m_world * m_camView * m_camProjection;
	m_cbPad.WVP = XMMatrixTranspose(m_WVP);
	BufferInitDesc cbbd;	

	cbbd.elementSize = sizeof(CBPad);
	cbbd.initData = NULL;
	cbbd.numElements = 1;
	cbbd.type = CONSTANT_BUFFER_VS;
	cbbd.usage = BUFFER_DEFAULT;
	
	m_cBuffer.init(m_pDevice, m_pDeviceContext, cbbd);
	m_padTexture = D3DTexture(m_pDevice, m_pDeviceContext);
	m_padTexture.createTexture(m_game.getPad()->getTexturePath(), 0);

	m_pDeviceContext->UpdateSubresource(m_cBuffer.getBufferPointer(), 0, NULL, &m_cbPad, 0, 0);
	m_cBuffer.apply(0);

	//## PAD END ##
	//## BLOCK START ##

	for(int i = 0; i < 4; i++)
	{
		BufferInitDesc blockBufferDesc;
		blockBufferDesc.elementSize		= sizeof(BlockVertex);
		blockBufferDesc.initData		= m_game.getField(i)->getBufferData();
		blockBufferDesc.numElements		= m_game.getField(i)->getListSize();
		blockBufferDesc.type			= VERTEX_BUFFER;
		blockBufferDesc.usage			= BUFFER_CPU_WRITE_DISCARD;

		m_blockBuffers[i].init(m_pDevice, m_pDeviceContext, blockBufferDesc);
	}


	D3D11_INPUT_ELEMENT_DESC blockInputdesc[] = 
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,	0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
	};
	m_blockShader.init(m_pDevice, m_pDeviceContext, 1);
	
	m_blockShader.compileAndCreateShaderFromFile(L"BlockShader.fx", "VS", "vs_5_0", VERTEX_SHADER, blockInputdesc);
	m_blockShader.compileAndCreateShaderFromFile(L"BlockShader.fx", "GS", "gs_5_0", GEOMETRY_SHADER, NULL);
	m_blockShader.compileAndCreateShaderFromFile(L"BlockShader.fx", "PS", "ps_5_0", PIXEL_SHADER, NULL);


	BufferInitDesc cBlockBufferDesc;	

	cBlockBufferDesc.elementSize = sizeof(cBlockBuffer);
	cBlockBufferDesc.initData = NULL;
	cBlockBufferDesc.numElements = 5;
	cBlockBufferDesc.type = CONSTANT_BUFFER_ALL;
	cBlockBufferDesc.usage = BUFFER_DEFAULT;
	
	m_cBlockBuffer.init(m_pDevice, m_pDeviceContext, cBlockBufferDesc);
	m_blockTexture = D3DTexture(m_pDevice, m_pDeviceContext);
	m_blockTexture.createTexture(m_game.getActiveField()->getBlock(0)->getTexturePath(),  0);

	//## BLOCK END ##
	//## BALL START ##
	m_ballBuffer =  Buffer();
	m_constantBallBuffer = Buffer();
	m_ballShader = Shader();

	m_ballShader.init(m_pDevice, m_pDeviceContext, 1);
	m_ballShader.compileAndCreateShaderFromFile(L"Ball.fx", "VShader", "vs_5_0", VERTEX_SHADER, desc);
	m_ballShader.compileAndCreateShaderFromFile(L"Ball.fx", "GShader", "gs_5_0", GEOMETRY_SHADER, NULL);
	m_ballShader.compileAndCreateShaderFromFile(L"Ball.fx", "PShader", "ps_5_0", PIXEL_SHADER, NULL);		

	bufferDesc.initData = &vec3(0,0,0);
	bufferDesc.numElements = 1;

	m_ballBuffer.init(m_pDevice, m_pDeviceContext, bufferDesc);
	m_ballTexture = D3DTexture(m_pDevice, m_pDeviceContext);
	m_ballTexture.createTexture(m_game.getBall()->getTexturePath(), 0);
	
	CBBall cbBall;

	BufferInitDesc cbBallDesc;
	cbBallDesc.elementSize = sizeof(CBBall);
	cbBallDesc.initData = NULL;
	cbBallDesc.numElements = 1;
	cbBallDesc.type = CONSTANT_BUFFER_ALL;
	cbBallDesc.usage = BUFFER_DEFAULT;
	
	m_constantBallBuffer.init(m_pDevice, m_pDeviceContext, cbBallDesc);
	m_pDeviceContext->UpdateSubresource(m_constantBallBuffer.getBufferPointer(), 0, NULL, &cbBall, 0, 0);
	m_constantBallBuffer.apply(0);

	D3D11_SAMPLER_DESC sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.Filter				= D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sd.AddressU				= D3D11_TEXTURE_ADDRESS_WRAP;
	sd.AddressV				= D3D11_TEXTURE_ADDRESS_WRAP;
	sd.AddressW				= D3D11_TEXTURE_ADDRESS_WRAP;
	sd.ComparisonFunc		= D3D11_COMPARISON_NEVER;
	sd.MinLOD				= 0;
	sd.MaxLOD				= D3D11_FLOAT32_MAX;

	m_pBallSampler = nullptr;
	hr = m_pDevice->CreateSamplerState( &sd, &m_pBallSampler );

	//## BALL END ##

	////## Bounding Volume DEBUGGING DRAW ##
	BoundingVolume* t_v;
	t_v = m_game.getPad()->getBoundingVolume();
	((AABB*)t_v)->initDraw(m_pDevice, m_pDeviceContext);
	for(int i = 0; i < 4; i++)
	{
		t_v = m_game.getActiveField()->getCollisionBorder(i);
		((AABB*)t_v)->initDraw(m_pDevice, m_pDeviceContext);
	}
	
	t_v = m_game.getBall()->getBoundingVolume();
	((Sphere*)t_v)->initDraw(m_pDevice, m_pDeviceContext);
	
	////## END DEBUGGING DRAW ##

	//## PLAYFIELD FINAL SETUP ##
	for (int i = 0; i  < 4; i ++)
	{
		m_game.getField(i)->transBorders(i % 2);
	}

	//## HID START ##
	m_HID = HID( getMainWnd() );
	// Add subscriber to the HID component. 
	m_HID.getObservable()->addSubscriber(m_game.getObserver());
	//## HID END ##

	//POWER UP START!
	BufferInitDesc vbDesc;
	vbDesc.elementSize = sizeof(Vertex);
	vbDesc.numElements = 4;
	vbDesc.type = VERTEX_BUFFER;
	vbDesc.usage = BUFFER_CPU_WRITE;

	Vertex initData[4];
	initData[0].m_position = vec3(-2.0f, -2.0f, 0.0f);
	initData[1].m_position = vec3(2.0f, -2.0f, 0.0f);
	initData[2].m_position = vec3(-2.0f, 2.0f, 0.0f);
	initData[3].m_position = vec3(2.0f, 2.0f, 0.0f);
	initData[0].m_textureCoordinates = vec2(1.0f, 1.0f);
	initData[1].m_textureCoordinates = vec2(0.0f, 1.0f);
	initData[2].m_textureCoordinates = vec2(1.0f, 0.0f);
	initData[3].m_textureCoordinates = vec2(0.0f, 0.0f);

	vbDesc.initData = initData;

	m_powerBuffer = Buffer();
	m_powerBuffer.init(m_pDevice, m_pDeviceContext, vbDesc);
	
	m_powerShader = Shader();
	m_powerShader.init(m_pDevice, m_pDeviceContext, 2);
	m_powerShader.compileAndCreateShaderFromFile(L"VertexShader.fx", "main", "vs_5_0", VERTEX_SHADER, desc);
	m_powerShader.compileAndCreateShaderFromFile(L"PixelShader.fx", "main", "ps_5_0", PIXEL_SHADER, NULL);

	m_powerTextures[FASTERBALL] = D3DTexture(m_pDevice, m_pDeviceContext);
	m_powerTextures[FASTERBALL].createTexture(new std::wstring(L"Picatures/fastAndFurious7.png"), 0);
	m_powerTextures[SLOWERBALL] = D3DTexture(m_pDevice, m_pDeviceContext);
	m_powerTextures[SLOWERBALL].createTexture(new std::wstring(L"Picatures/slow.png"), 0);
	m_powerTextures[BIGGERPAD] = D3DTexture(m_pDevice, m_pDeviceContext);
	m_powerTextures[BIGGERPAD].createTexture(new std::wstring(L"Picatures/biggerPad.png"), 0);
	m_powerTextures[SMALLERPAD] = D3DTexture(m_pDevice, m_pDeviceContext);
	m_powerTextures[SMALLERPAD].createTexture(new std::wstring(L"Picatures/smallerPad.png"), 0);
	m_powerTextures[STICKYPAD] = D3DTexture(m_pDevice, m_pDeviceContext);
	m_powerTextures[STICKYPAD].createTexture(m_game.getBall()->getTexturePath(), 0);

	D3D11_BLEND_DESC bd;
	bd.AlphaToCoverageEnable = false;
	bd.IndependentBlendEnable = false;
	bd.RenderTarget[0].BlendEnable = true;
	bd.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	bd.RenderTarget[0].DestBlend =  D3D11_BLEND_INV_SRC_ALPHA;
	bd.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	bd.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_SRC_ALPHA;
	bd.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_DEST_ALPHA;
	bd.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	bd.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	m_pDevice->CreateBlendState(&bd, &m_pPowerBlend);
	m_powerShader.setBlendState(m_pPowerBlend);
	m_pBallSampler = nullptr;

	hr = m_pDevice->CreateSamplerState( &sd, &m_pBallSampler );
	//POWER UP END!
}

void Direct3D::onResize()
{
	D3DApp::onResize();
}

void Direct3D::updateScene(float p_dt)
{
	D3DApp::updateScene(p_dt);
	m_game.update(m_ScreenViewport.Width, p_dt);
	m_camView = mat4ToXMMatrix(m_camera->getViewMatrix());
	m_camProjection = mat4ToXMMatrix(m_camera->getProjectionMatrix());
	m_camPosition = vec3ToXMVector(m_camera->getPosition());
	m_camera->updateCameraPos();
	
	//Update Active block buffer
	unsigned int active = m_game.getActiveFieldNr();
	if(m_game.getField(active)->getUpdateBuffer())
	{
		m_blockBuffers[active].map();
		D3D11_MAPPED_SUBRESOURCE* ms = m_blockBuffers[active].getMappedResource();
		int u = sizeof(BlockVertex);

		memcpy(ms->pData, m_game.getField(active)->getBufferData(), u *m_game.getField(active)->getListSize() );

		m_blockBuffers[active].unmap();
		
		m_game.getField(active)->setUpdateBuffer(false);
	}

	float x,y;
	x = ((Pad*)(m_game.getPad()))->getMousePos().x;
	y = ((Pad*)(m_game.getPad()))->getMousePos().y;

	std::wostringstream outs;   
	outs.precision(6);
	outs << L"    PAD:"
		<< L" " << m_game.getPad()->getBoundingVolume()->getPosition()->x << L"," 
		<< m_game.getPad()->getBoundingVolume()->getPosition()->y << L","
		<< m_game.getPad()->getBoundingVolume()->getPosition()->z << L","
		
		<< L"    MP:" << L" " << x << L"," << y  << L","

		<< L"    BALL:"
		<< L" " << m_game.getBall()->getBoundingVolume()->getPosition()->x << L"," 
		<< m_game.getBall()->getBoundingVolume()->getPosition()->y << L","
		<< m_game.getBall()->getBoundingVolume()->getPosition()->z << L",";

		

	SetWindowTextW(m_hMainWnd,outs.str().c_str());

	m_winTitle = outs.str();
}

void Direct3D::drawScene()
{
	D3DApp::drawScene();
	cBlockBuffer cBlockBufferStruct;

	XMMATRIX playFieldRotation = mat4ToXMMatrix(m_game.getActiveField()->getRotationMatrix());
	
	//// Bounding Volume DEBUGGING DRAW
	BoundingVolume* t_v;
	t_v = m_game.getBall()->getBoundingVolume();
	Sphere t_sphere = *((Sphere*)t_v);
	t_sphere.draw(m_world, m_camView, m_camProjection);
	t_v = m_game.getPad()->getBoundingVolume();
	AABB t_bb = *((AABB*)t_v);
	t_bb.draw(m_world, m_camView, m_camProjection);
	for(int i = 0; i < 4; i++)
	{
		t_v = m_game.getActiveField()->getCollisionBorder(i);
		AABB t_bb = *((AABB*)t_v);
		t_bb.draw(m_world, m_camView, m_camProjection);
	}
	//// END DEBUGGING DRAW

	//## PAD DRAW START ##
	XMMATRIX translatePadMatrix;

	vec3 padPos = ((Pad*)(m_game.getPad()))->getRealPosition();
	translatePadMatrix = XMMatrixTranslation(padPos.x, padPos.y, padPos.z);

	m_world = XMMatrixIdentity();
	
	XMMATRIX t_scaleMatrix = XMMatrixIdentity() * ((Pad*)(m_game.getPad()))->getScale();
	t_scaleMatrix.r[3].m128_f32[3] = 1.0f;
	m_WVP = m_world * playFieldRotation * t_scaleMatrix * translatePadMatrix * m_camView * m_camProjection;

	m_cbPad.WVP = XMMatrixTranspose(m_WVP);
	m_cBuffer.apply(0);
	m_pDeviceContext->UpdateSubresource(m_cBuffer.getBufferPointer(), 0, NULL, &m_cbPad, 0, 0);
	m_shader.setShaders();
	m_shader.setResource(PIXEL_SHADER, 0, 1, m_padTexture.getResourceView());
	m_shader.setSamplerState(PIXEL_SHADER, 0, 1, m_pBallSampler);
	m_pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	m_buffer.apply(0);
	m_pDeviceContext->Draw(4, 0);
	//## PAD DRAW END ##

	//## BLOCK DRAW START ##
	m_WVP = m_world *m_camView * m_camProjection;
	cBlockBufferStruct.WVP = XMMatrixTranspose(m_WVP);
	m_pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);
	m_blockShader.setShaders();
	m_blockShader.setResource(PIXEL_SHADER, 0, 1, m_blockTexture.getResourceView());
	m_blockShader.setSamplerState(PIXEL_SHADER, 0, 1, m_pBallSampler);
	cBlockBufferStruct.sizeX = g_bvSize.x;
	cBlockBufferStruct.sizeY = g_bvSize.y;
	cBlockBufferStruct.sizeZ = g_bvSize.z;
	m_pDeviceContext->UpdateSubresource(m_cBlockBuffer.getBufferPointer(), 0, NULL, &cBlockBufferStruct, 0, 0);
	m_cBlockBuffer.apply(0);
	unsigned int active = m_game.getActiveFieldNr();

	for(int i = 0; i < 4; i++)
	{
		cBlockBufferStruct.rotation = XMMatrixTranspose( mat4ToXMMatrix(m_game.getField(i)->getRotationMatrix()));
		m_pDeviceContext->UpdateSubresource(m_cBlockBuffer.getBufferPointer(), 0, NULL, &cBlockBufferStruct, 0, 0);
		m_blockBuffers[i].apply(0);
		m_pDeviceContext->Draw(m_game.getField(i)->getListSize(), 0);
	}
	//## BLOCK DRAW END ##
	
	//## POWERUP DRAW START ##
	if(m_powerUps.size() > 0)
	{
		m_powerShader.setShaders();
		m_pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
		m_powerShader.setSamplerState(PIXEL_SHADER, 0, 1, m_pBallSampler);

		for(unsigned int i = 0; i < m_powerUps.size(); i++)
		{
			PowerUp* pu;
			pu = m_powerUps.at(i);
			if (active == 0 || active == 2)
				translatePadMatrix = XMMatrixTranslation(pu->getPos()->x, pu->getPos()->y, padPos.z); // Translate powerup
			else
				translatePadMatrix = XMMatrixTranslation(padPos.x, pu->getPos()->y, pu->getPos()->z);
			m_WVP = m_world * playFieldRotation * translatePadMatrix * m_camView * m_camProjection;
			m_cbPad.WVP = XMMatrixTranspose(m_WVP);
			m_cBuffer.apply(0);
			m_pDeviceContext->UpdateSubresource(m_cBuffer.getBufferPointer(), 0, NULL, &m_cbPad, 0, 0);
			m_powerShader.setResource(PIXEL_SHADER, 0, 1, m_powerTextures[pu->getType()].getResourceView());
			m_powerBuffer.apply();
			m_pDeviceContext->Draw(4, 0);

			//t_v = pu->getBoundingVolume();
			//t_bb = *((AABB*)t_v);
			//t_bb.initDraw(m_pDevice, m_pDeviceContext);
			//t_bb.draw(m_world, m_camView, m_camProjection);
		}
	}
	//## POWERUP DRAW END ##

	//## BALL DRAW START ##
	vec3 t_ballPos = ((Ball*)m_game.getBall())->getRealPosition();

	m_cbBall.eyePosW = m_camPosition;
	m_cbBall.viewProj = XMMatrixTranspose(m_camView * m_camProjection);
	m_cbBall.translation = XMMatrixTranspose(XMMatrixTranslation(t_ballPos.x, t_ballPos.y, t_ballPos.z));
	m_cbBall.size = XMFLOAT2(5.0f, 5.0f);
	m_constantBallBuffer.apply(0);

	m_pDeviceContext->UpdateSubresource(m_constantBallBuffer.getBufferPointer(), 0, NULL, &m_cbBall, 0, 0);
	m_ballShader.setShaders();
	m_ballShader.setResource(PIXEL_SHADER, 0, 1, m_ballTexture.getResourceView());
	m_ballShader.setSamplerState(PIXEL_SHADER, 0, 1, m_pBallSampler);
	
	m_pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);
	m_ballBuffer.apply(0);

	m_pDeviceContext->Draw(1, 0);
	//## BALL DRAW END ##

	m_pSwapChain->Present(0, 0);
}

LRESULT Direct3D::msgProc(UINT p_msg, WPARAM p_wParam, LPARAM p_lParam)
{
	m_HID.update(p_msg, p_lParam);

	return D3DApp::msgProc(p_msg, p_wParam, p_lParam);;
}

void Direct3D::addPowerUp(PowerUp* p_pPowerUp)
{
	m_powerUps.push_back(p_pPowerUp);
	//Update buffer
}

void Direct3D::removePowerUp(int i)
{
	m_powerUps.erase(m_powerUps.begin() + i);
}