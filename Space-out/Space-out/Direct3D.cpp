#include "Direct3D.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance,
				   PSTR cmdLine, int showCmd)
{
	// Enable run-time memory check for debug builds.
#if defined(DEBUG) | defined(_DEBUG)
	_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
#endif


	Direct3D theApp(hInstance);
	
	theApp.initApp();

	return theApp.run();
}
struct cbPerObj
{
	XMMATRIX WVP;
};
Direct3D::Direct3D(HINSTANCE hInstance)
: D3DApp(hInstance) 
{

}

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
	m_buffer = Buffer();
	m_cBuffer = Buffer();
	m_shader = Shader();

	Vertex data[4];
	data[0].pos = XMFLOAT3(-10.0f,0.0f,0.0f);
	data[1].pos = XMFLOAT3(10.0f,0.0f,0.0f);
	data[2].pos = XMFLOAT3(-10.0f,10.0f,0.0f);
	data[3].pos = XMFLOAT3(10.0f,10.0f,0.0f);

	BufferInitDesc bufferDesc;
	bufferDesc.elementSize = sizeof(Vertex);
	bufferDesc.initData = data;
	bufferDesc.numElements = 4;
	bufferDesc.type = VERTEX_BUFFER;
	bufferDesc.usage = BUFFER_DEFAULT;
	
	m_buffer.init(m_pDevice, m_pDeviceContext, bufferDesc);
	
	

	D3D11_INPUT_ELEMENT_DESC desc[] = 
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};
	
	m_shader.init(m_pDevice, m_pDeviceContext, 1);
	m_shader.compileAndCreateShaderFromFile(L"VertexShader.fx", "main","vs_5_0", VERTEX_SHADER , desc);
	m_shader.compileAndCreateShaderFromFile(L"PixelShader.fx", "main", "ps_5_0", PIXEL_SHADER, NULL);

	m_HID = HID( getMainWnd() );
	
	//Set up world view projdf
	camPosition = XMVectorSet( 0.0f, 0.0f, -20.f, 0.0f );
	camTarget = XMVectorSet( 0.0f, 0.0f, 0.0f, 0.0f );
	camUp = XMVectorSet( 0.0f, 1.0f, 0.0f, 0.0f );

	camView = XMMatrixLookAtLH( camPosition, camTarget, camUp );

	camProjection = XMMatrixPerspectiveFovLH( 0.4f*3.14f, (float)m_ClientWidth/m_ClientHeight, 1.0f, 1000.0f);
	
	world = XMMatrixIdentity();
	WVP	= world * camView * camProjection;
	cBufferStruct.WVP = XMMatrixTranspose(WVP);

	BufferInitDesc cbbd;	

	cbbd.elementSize = sizeof(cbPerObj);
	cbbd.initData = NULL;
	cbbd.numElements = 1;
	cbbd.type = CONSTANT_BUFFER_VS;
	cbbd.usage = BUFFER_CPU_WRITE_DISCARD;
	
	m_cBuffer.init(m_pDevice, m_pDeviceContext, cbbd);

	m_pDeviceContext->UpdateSubresource(m_cBuffer.getBufferPointer(), 0, NULL, &cBufferStruct, 0, 0);

	m_game = Game();
	m_game.init();

	// Add subscriber to the HID component. 
	m_HID.getObservable()->addSubscriber(m_game.getObserver());
}

void Direct3D::onResize()
{
	D3DApp::onResize();
}

void Direct3D::updateScene(float dt)
{
	D3DApp::updateScene(dt);
	m_game.update();
}

void Direct3D::drawScene()
{
	
	D3DApp::drawScene();
	cbPerObj cBufferStruct;

	m_buffer.apply(0);
	m_pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);

	m_shader.setShaders();

	world = XMMatrixIdentity();

	WVP = world * camView * camProjection;

	cBufferStruct.WVP = XMMatrixTranspose(WVP);

	m_pDeviceContext->UpdateSubresource(m_cBuffer.getBufferPointer(), 0, NULL, &cBufferStruct, 0, 0);


	m_pDeviceContext->Draw(10, 0);

	m_pSwapChain->Present(0, 0);
}

LRESULT Direct3D::msgProc(UINT msg, WPARAM wParam, LPARAM lParam)
{
	m_HID.update(msg, lParam);
	/*switch( msg )
	{
		return 0;
	}*/

	return D3DApp::msgProc(msg, wParam, lParam);
}