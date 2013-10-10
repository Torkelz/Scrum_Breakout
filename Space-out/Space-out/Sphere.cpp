#include "Sphere.h"

Sphere::Sphere()
{
	setRadius(0.0f);
	m_position = vec3(0,0,0);

	m_type = SPHERE;
}

Sphere::Sphere(float p_radius, vec3 p_pos) : BoundingVolume()
{
	setRadius(p_radius);
	m_position	= p_pos;

	m_type = SPHERE;
}

Sphere::~Sphere(){}

float Sphere::getRadius()
{
	return m_radius;
}

float Sphere::getSqrRadius()
{
	return m_sqrRadius;
}

void Sphere::setRadius(float p_radius)
{
	m_radius = p_radius;
	m_sqrRadius = m_radius * m_radius;
}

bool Sphere::sphereVsSphere (Sphere* p_sphere)
{
    vec3 CDiff;
	CDiff = *p_sphere->getPosition() - m_position;
	
	float c = length2(CDiff);
	float rSum = p_sphere->getRadius() + m_radius;
    float rSumSqr = rSum*rSum;

    return c <= rSumSqr;
}

////DEBUGGING
void Sphere::initDraw(ID3D11Device* p_pDevice, ID3D11DeviceContext* p_pDeviceContext)
{
	m_pDevice = p_pDevice;
	m_pDeviceContext = p_pDeviceContext;

	BufferInitDesc desc;
	desc.type				= VERTEX_BUFFER;
	desc.numElements		= 1;
	desc.elementSize		= sizeof( XMFLOAT3 );
	desc.usage				= BUFFER_DEFAULT;
	//desc.initData			= &XMFLOAT3(m_position.x, m_position.y, m_position.z);
	desc.initData			= &XMFLOAT3(0, 0, 0);
	m_pBuffer = new Buffer();
	m_pBuffer->init(p_pDevice, p_pDeviceContext, desc);

	BufferInitDesc cbDesc;

	cbDesc.elementSize = sizeof(CBS);
	cbDesc.initData = NULL;
	cbDesc.numElements = 1;
	cbDesc.type = CONSTANT_BUFFER_VS;
	cbDesc.usage = BUFFER_DEFAULT;
	
	m_pCB = new Buffer();
	m_pCB->init(p_pDevice, p_pDeviceContext, cbDesc);

	D3D11_INPUT_ELEMENT_DESC inputDesc[] = 
	{
		{"POS", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};

	m_pShader = new Shader();
	m_pShader->init(p_pDevice, p_pDeviceContext, 1);
	m_pShader->compileAndCreateShaderFromFile(L"BoundingBox.fx", "VSInstmain","vs_5_0", VERTEX_SHADER , inputDesc);
	m_pShader->compileAndCreateShaderFromFile(L"BoundingBox.fx", "PSScene", "ps_5_0", PIXEL_SHADER, NULL);
}

void Sphere::draw(XMMATRIX& p_world, XMMATRIX& p_view, XMMATRIX& p_proj)
{
	XMMATRIX WorldViewProj;
	m_translate = XMMatrixTranslation(m_position.x, m_position.y, m_position.z);
	WorldViewProj = XMMatrixMultiply(p_world, m_translate);
	WorldViewProj = XMMatrixMultiply(WorldViewProj, p_view );
	WorldViewProj = XMMatrixMultiply(WorldViewProj, p_proj );

	m_pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);

	m_cb.color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	m_cb.WVP = XMMatrixTranspose(WorldViewProj);
	m_pCB->apply(0);
	m_pDeviceContext->UpdateSubresource(m_pCB->getBufferPointer(), 0, NULL, &m_cb, 0, 0);
	
	m_pBuffer->apply(0);
	m_pShader->setShaders();

	m_pDeviceContext->Draw(1, 0);
}