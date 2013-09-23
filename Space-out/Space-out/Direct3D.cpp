#include "Direct3D.h"
#include "Object.h"
#include "ABlock.h"

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
struct cbPerObj
{
	XMMATRIX WVP;
};
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
	cbPerObj cBufferStruct;

	m_buffer			= Buffer();
	m_cBuffer			= Buffer();
	m_shader			= Shader();

	for(int i = 0; i < 4; i++)
		m_blockBuffers[i]	= Buffer();

	m_blockShader		= Shader();
	m_cBlockBuffer		= Buffer();

	m_game = Game();
	m_game.init();

	
	// PAD
	UINT32 const nrVertices = 4;
	Vector3 data[nrVertices];
	std::vector<Vector3>* t_data = m_game.getPad()->getVertices();


	for(int i = 0; i < nrVertices; i++)
	{
		data[i] = t_data->at(i);
	}

	BufferInitDesc bufferDesc;
	bufferDesc.elementSize = sizeof(Vector3);
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

	// PAD END


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
	



	m_HID = HID( getMainWnd() );
	
	//Set up world view projdf
	m_camPosition = XMVectorSet( 45.0f, 0.0f, -60.0f, 0.0f );
	m_camTarget = XMVectorSet( 45.0f, 0.0f, 0.0f, 0.0f );
	m_camUp = XMVectorSet( 0.0f, 1.0f, 0.0f, 0.0f );

	m_camView = XMMatrixLookAtLH( m_camPosition, m_camTarget, m_camUp );

	m_camProjection = XMMatrixPerspectiveFovLH( 0.4f*3.14f, (float)m_ClientWidth/m_ClientHeight, 1.0f, 1000.0f);
	
	m_world = XMMatrixIdentity();
	m_WVP	= m_world * m_camView * m_camProjection;
	cBufferStruct.WVP = XMMatrixTranspose(m_WVP);

	BufferInitDesc cbbd;	

	cbbd.elementSize = sizeof(cbPerObj);
	cbbd.initData = NULL;
	cbbd.numElements = 1;
	cbbd.type = CONSTANT_BUFFER_ALL;
	cbbd.usage = BUFFER_DEFAULT;
	
	m_cBuffer.init(m_pDevice, m_pDeviceContext, cbbd);

	m_pDeviceContext->UpdateSubresource(m_cBuffer.getBufferPointer(), 0, NULL, &cBufferStruct, 0, 0);
	m_cBuffer.apply(0);



	// Add subscriber to the HID component. 
	m_HID.getObservable()->addSubscriber(m_game.getObserver());
}

void Direct3D::onResize()
{
	D3DApp::onResize();
}

void Direct3D::updateScene(float p_dt)
{
	D3DApp::updateScene(p_dt);
	m_game.update();
}

void Direct3D::drawScene()
{
	D3DApp::drawScene();
	cbPerObj cBufferStruct;
	cBlockBuffer cBlockBufferStruct;

	XMMATRIX translatePadMatrix;
	
	Vector3* t_pos = m_game.getPad()->getPos();

	//Try to get the pad closer to the actual mouse.
	float tempX = t_pos->x - m_ScreenViewport.Width/2;

	translatePadMatrix = XMMatrixTranslation(tempX, t_pos->y, t_pos->z);

	m_world = XMMatrixIdentity();
	m_WVP = translatePadMatrix * m_world * m_camView * m_camProjection;

	cBufferStruct.WVP = XMMatrixTranspose(m_WVP);
	m_pDeviceContext->UpdateSubresource(m_cBuffer.getBufferPointer(), 0, NULL, &cBufferStruct, 0, 0); 
	m_cBuffer.apply(0);
	m_shader.setShaders();
	m_pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	m_buffer.apply(0);

	m_pDeviceContext->Draw(4, 0);

	//TEST BLOCK DRAW!
	
	m_WVP = m_world *m_camView * m_camProjection;
	cBlockBufferStruct.WVP = XMMatrixTranspose(m_WVP);
	m_pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);
	m_blockShader.setShaders();

	cBlockBufferStruct.sizeX = g_bSizeX;
	cBlockBufferStruct.sizeY = g_bSizeY;
	cBlockBufferStruct.sizeZ = g_bSizeZ;
	m_pDeviceContext->UpdateSubresource(m_cBlockBuffer.getBufferPointer(), 0, NULL, &cBlockBufferStruct, 0, 0);
	m_cBlockBuffer.apply(0);
	m_blockBuffers[0].apply(0);
	m_pDeviceContext->Draw(m_blockBufferSizeF, 0);
	m_blockBuffers[1].apply(0);
	m_pDeviceContext->Draw(m_blockBufferSizeB, 0);

	cBlockBufferStruct.sizeX = g_bSizeZ;
	cBlockBufferStruct.sizeY = g_bSizeY;
	cBlockBufferStruct.sizeZ = g_bSizeX;
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