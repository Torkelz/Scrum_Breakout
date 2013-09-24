#include "Direct3D.h"
#include "Object.h"
#include "ABlock.h"
#include "Ball.h"

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
	m_game = Game();
	m_game.init();


	//Set up world view projdf
	m_camPosition = XMVectorSet( 0.0f, 0.0f, -100.f, 0.0f );
	m_camTarget = XMVectorSet( 0.0f, 0.0f, 0.0f, 0.0f );
	m_camUp = XMVectorSet( 0.0f, 1.0f, 0.0f, 0.0f );

	m_camView = XMMatrixLookAtLH( m_camPosition, m_camTarget, m_camUp );
	m_camProjection = XMMatrixPerspectiveFovLH( 0.4f*3.14f, (float)m_ClientWidth/m_ClientHeight, 1.0f, 1000.0f);
	
	m_world = XMMatrixIdentity();
	
	// PAD ###
	UINT32 const nrVertices = 4;
	vec3 data[nrVertices];
	std::vector<vec3>* t_data = m_game.getPad()->getVertices();

	for(int i = 0; i < nrVertices; i++)
	{
		data[i] = t_data->at(i);
	}

	BufferInitDesc bufferDesc;
	bufferDesc.elementSize = sizeof(vec3);
	bufferDesc.initData = &data;
	bufferDesc.numElements = nrVertices;
	bufferDesc.type = VERTEX_BUFFER;
	bufferDesc.usage = BUFFER_USAGE_IMMUTABLE;
	
	m_buffer.init(m_pDevice, m_pDeviceContext, bufferDesc);
	
	D3D11_INPUT_ELEMENT_DESC desc[] = 
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};
	
	m_shader.init(m_pDevice, m_pDeviceContext, 1);
	m_shader.compileAndCreateShaderFromFile(L"VertexShader.fx", "main","vs_5_0", VERTEX_SHADER , desc);
	m_shader.compileAndCreateShaderFromFile(L"PixelShader.fx", "main", "ps_5_0", PIXEL_SHADER, NULL);


	//TEST BLOCK START
	for(int i = 0; i < 4; i++)
	{
		std::vector<BlockVertex> blockData;
		m_pBlockList = m_game.getBlocks(i);
		int listSize = m_pBlockList->size();

		for (int j = 0; j < listSize; j++)
			blockData.push_back(m_pBlockList->at(j)->getBlockVertex());

		BufferInitDesc blockBufferDesc;
		blockBufferDesc.elementSize		= sizeof(BlockVertex);
		blockBufferDesc.initData		= blockData.data();
		blockBufferDesc.numElements		= listSize;
		blockBufferDesc.type			= VERTEX_BUFFER;
		blockBufferDesc.usage			= BUFFER_USAGE_IMMUTABLE;

		m_blockBuffers[i].init(m_pDevice, m_pDeviceContext, blockBufferDesc);
		if(i == 0)
			m_blockBufferSizeF = listSize;
		else if(i == 1)
			m_blockBufferSizeB = listSize;
		else if(i == 2)
			m_blockBufferSizeL = listSize;
		else
			m_blockBufferSizeR = listSize;
	}


	D3D11_INPUT_ELEMENT_DESC blockInputdesc[] = 
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,	0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"COLOR",	 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
	};
	m_blockShader.init(m_pDevice, m_pDeviceContext, 2);
	
	m_blockShader.compileAndCreateShaderFromFile(L"BlockShader.fx", "VS", "vs_5_0", VERTEX_SHADER, blockInputdesc);
	m_blockShader.compileAndCreateShaderFromFile(L"BlockShader.fx", "GS", "gs_5_0", GEOMETRY_SHADER, NULL);
	m_blockShader.compileAndCreateShaderFromFile(L"BlockShader.fx", "PS", "ps_5_0", PIXEL_SHADER, NULL);


	BufferInitDesc cBlockBufferDesc;	

	cBlockBufferDesc.elementSize = sizeof(cBlockBuffer);
	cBlockBufferDesc.initData = NULL;
	cBlockBufferDesc.numElements = 4;
	cBlockBufferDesc.type = CONSTANT_BUFFER_ALL;
	cBlockBufferDesc.usage = BUFFER_DEFAULT;
	
	m_cBlockBuffer.init(m_pDevice, m_pDeviceContext, cBlockBufferDesc);

	//TEST BLOCK END
	
	m_WVP	= m_world * m_camView * m_camProjection;
	m_cbPad.WVP = XMMatrixTranspose(m_WVP);

	BufferInitDesc cbbd;	

	cbbd.elementSize = sizeof(CBPad);
	cbbd.initData = NULL;
	cbbd.numElements = 1;
	cbbd.type = CONSTANT_BUFFER_VS;
	cbbd.usage = BUFFER_DEFAULT;
	
	m_cBuffer.init(m_pDevice, m_pDeviceContext, cbbd);

	m_pDeviceContext->UpdateSubresource(m_cBuffer.getBufferPointer(), 0, NULL, &m_cbPad, 0, 0);
	m_cBuffer.apply(0);



	// PAD END ###

	// BALLZZZ TO THE WALL
	m_ballBuffer =  Buffer();
	m_constantBallBuffer = Buffer();
	m_ballShader = Shader();

	m_ballShader.init(m_pDevice, m_pDeviceContext, 1);
	m_ballShader.compileAndCreateShaderFromFile(L"Ball.fx", "VShader", "vs_5_0", VERTEX_SHADER, desc);
	m_ballShader.compileAndCreateShaderFromFile(L"Ball.fx", "GShader", "gs_5_0", GEOMETRY_SHADER, NULL);
	m_ballShader.compileAndCreateShaderFromFile(L"Ball.fx", "PShader", "ps_5_0", PIXEL_SHADER, NULL);		

	bufferDesc.initData = m_game.getBall()->getPos();
	bufferDesc.numElements = 1;

	m_ballBuffer.init(m_pDevice, m_pDeviceContext, bufferDesc);

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

	// BALLZZZ FROM THE WALL

	// Bounding Volume DEBUGGING DRAW
	BoundingVolume* t_v;
	t_v = m_game.getPad()->getBoundingVolume();
	((AABB*)t_v)->initDraw(m_pDevice, m_pDeviceContext);

	t_v = m_game.getBall()->getBoundingVolume();
	((Sphere*)t_v)->initDraw(m_pDevice, m_pDeviceContext);
	// END DEBUGGING DRAW

	// HID-STUFF

	m_HID = HID( getMainWnd() );
	// Add subscriber to the HID component. 
	m_HID.getObservable()->addSubscriber(m_game.getObserver());

	// END HID-STUFF
}

void Direct3D::onResize()
{
	D3DApp::onResize();
}

void Direct3D::updateScene(float p_dt)
{
	D3DApp::updateScene(p_dt);
	m_game.update(m_ScreenViewport.Width);
}

void Direct3D::drawScene()
{
	D3DApp::drawScene();
	cBlockBuffer cBlockBufferStruct;

	XMMATRIX translatePadMatrix;
	
	vec3* t_pos = m_game.getPad()->getPos();

	//Try to get the pad closer to the actual mouse.
	float tempX = t_pos->x - m_ScreenViewport.Width/2;

	translatePadMatrix = XMMatrixTranslation(tempX, t_pos->y, t_pos->z);

	m_world = XMMatrixIdentity();
	m_WVP = translatePadMatrix * m_world * m_camView * m_camProjection;

	m_cbPad.WVP = XMMatrixTranspose(m_WVP);
	m_cBuffer.apply(0);
	m_pDeviceContext->UpdateSubresource(m_cBuffer.getBufferPointer(), 0, NULL, &m_cbPad, 0, 0);
	m_shader.setShaders();
	m_pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	m_buffer.apply(0);
	m_pDeviceContext->Draw(4, 0);

	// Ball draw shit
	vec3* t_ballPos = m_game.getBall()->getPos();

	m_cbBall.eyePosW = m_camPosition;
	m_cbBall.viewProj = XMMatrixTranspose(m_camView * m_camProjection);
	m_cbBall.translation = XMMatrixTranspose(XMMatrixTranslation(t_ballPos->x, t_ballPos->y, t_ballPos->z));
	m_cbBall.size = XMFLOAT2(5.0f, 5.0f);
	m_constantBallBuffer.apply(0);

	m_pDeviceContext->UpdateSubresource(m_constantBallBuffer.getBufferPointer(), 0, NULL, &m_cbBall, 0, 0);
	m_ballShader.setShaders();
	m_pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);

	m_ballBuffer.apply(0);
	m_pDeviceContext->Draw(1, 0);
	// end shit

	// Bounding Volume DEBUGGING DRAW
	BoundingVolume* t_v;
	t_v = m_game.getBall()->getBoundingVolume();
	Sphere t_sphere = *((Sphere*)t_v);
	t_sphere.draw(m_world, m_camView, m_camProjection);

	t_v = m_game.getPad()->getBoundingVolume();
	AABB t_bb = *((AABB*)t_v);
	t_bb.draw(m_world, m_camView, m_camProjection);
	// END DEBUGGING DRAW

	//TEST BLOCK DRAW!
	
	m_WVP = m_world *m_camView * m_camProjection;
	cBlockBufferStruct.WVP = XMMatrixTranspose(m_WVP);
	m_pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);
	m_blockShader.setShaders();

	cBlockBufferStruct.sizeX = g_bvSize.x;
	cBlockBufferStruct.sizeY = g_bvSize.y;
	cBlockBufferStruct.sizeZ = g_bvSize.z;
	m_pDeviceContext->UpdateSubresource(m_cBlockBuffer.getBufferPointer(), 0, NULL, &cBlockBufferStruct, 0, 0);
	m_cBlockBuffer.apply(0);
	m_blockBuffers[0].apply(0);
	m_pDeviceContext->Draw(m_blockBufferSizeF, 0);
	m_blockBuffers[1].apply(0);
	m_pDeviceContext->Draw(m_blockBufferSizeB, 0);

	cBlockBufferStruct.sizeX = g_bvSize.z;
	cBlockBufferStruct.sizeY = g_bvSize.y;
	cBlockBufferStruct.sizeZ = g_bvSize.x;
	m_pDeviceContext->UpdateSubresource(m_cBlockBuffer.getBufferPointer(), 0, NULL, &cBlockBufferStruct, 0, 0);
	m_cBlockBuffer.apply(0);
	m_blockBuffers[2].apply(0);
	m_pDeviceContext->Draw(m_blockBufferSizeL, 0);
	m_blockBuffers[3].apply(0);
	m_pDeviceContext->Draw(m_blockBufferSizeR, 0);
	
	m_pSwapChain->Present(1, 0);
}

LRESULT Direct3D::msgProc(UINT p_msg, WPARAM p_wParam, LPARAM p_lParam)
{
	m_HID.update(p_msg, p_lParam);

	return D3DApp::msgProc(p_msg, p_wParam, p_lParam);;
}