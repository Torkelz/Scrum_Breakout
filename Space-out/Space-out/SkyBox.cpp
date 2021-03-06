#include "SkyBox.h"


SkyBox::SkyBox()//ID3D10Device* pDevice)
{
	//mDevice = pDevice;
	m_NumIndices = 0;
}

SkyBox::~SkyBox()
{
	/*mVertexBuffer->~Buffer();
	mVertexBuffer = NULL;
	mIndexBuffer->~Buffer();
	mIndexBuffer = NULL;
	mShader->~Shader();
	mShader = NULL;*/
}

bool SkyBox::init(float p_Radius)
{
	

	BuildGeoSphere( 2, p_Radius);

	vector<vec3> initSkym_vertices(m_vertices.size());
	for(unsigned int i = 0; i < m_vertices.size(); i++)
	{
		initSkym_vertices[i] = 0.5f * m_vertices[i];
	}

	////Create Vertex Buffer Description
	//BUFFER_INIT_DESC bdVertex;
	//bdVertex.ElementSize = sizeof(SkyVertex);
	//bdVertex.InitData = initSkym_vertices.data();
	//bdVertex.NumElements = initSkym_vertices.size();
	//bdVertex.Type = VERTEX_BUFFER;
	//bdVertex.Usage = BUFFER_DEFAULT; //BUFFER_DEFAULT
	//
	////Create Buffer
	//mVertexBuffer = new Buffer();
	//if(FAILED(mVertexBuffer->Init(mDevice, bdVertex)))
	//{
	//	return E_FAIL;
	//}

	//mNumIndices = (unsigned int)m_initIndices.size();

	////Index Buffer Description
	//BUFFER_INIT_DESC bdIndex;
	//bdIndex.ElementSize = sizeof( unsigned int );
	//bdIndex.InitData = m_initIndices.data();
	//bdIndex.NumElements = mNumIndices;
	//bdIndex.Type = INDEX_BUFFER;
	//bdIndex.Usage = BUFFER_DEFAULT; //BUFFER_DEFAULT
	//
	////Create Index Buffer
	//mIndexBuffer = new Buffer();
	//if(FAILED(mIndexBuffer->Init(mDevice, bdIndex)))
	//{
	//	return E_FAIL;
	//}

	//// Define our vertex data layout
	//const D3D10_INPUT_ELEMENT_DESC VertexLayout[] =
	//{
	//	{ "POS", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D10_INPUT_PER_VERTEX_DATA, 0 }
	//};

	////Create Shader
	//mShader = new Shader();
	//if(FAILED(mShader->Init(mDevice, "SkyBox.fx", VertexLayout, sizeof(VertexLayout) / sizeof(D3D10_INPUT_ELEMENT_DESC), "RenderSky",
	//	D3D10_SHADER_ENABLE_BACKWARDS_COMPATIBILITY)))
	//{
	//	return E_FAIL;
	//}

	////Load Cube Map
	//D3DX10_IMAGE_LOAD_INFO loadInfo;
	//loadInfo.MiscFlags = D3D10_RESOURCE_MISC_TEXTURECUBE;

	//ID3D10Texture2D* tex = 0;

	//if(FAILED(D3DX10CreateTextureFromFile(mDevice, "files\\textures\\grassenvmap1024.dds",
	//&loadInfo, 0, (ID3D10Resource**)&tex, 0)))
	//{
	//	return E_FAIL;
	//}

	////Send to .fx
	//D3D10_TEXTURE2D_DESC texDesc;
	//tex->GetDesc(&texDesc);

	//D3D10_SHADER_RESOURCE_VIEW_DESC viewDesc;
	//viewDesc.Format = texDesc.Format;
	//viewDesc.ViewDimension = D3D10_SRV_DIMENSION_TEXTURECUBE;
	//viewDesc.TextureCube.MipLevels = texDesc.MipLevels;
	//viewDesc.TextureCube.MostDetailedMip = 0;

	//
	//if(FAILED(mDevice->CreateShaderResourceView(tex, &viewDesc, &mRV)))
	//{
	//	return E_FAIL;
	//}
	//mShader->SetResource("gCubeMap", mRV);
	//SAFE_RELEASE(tex);

	return true;
}

//void SkyBox::prepareToRender(D3DXMATRIX& pWorld, vec3 pEyePos)//, D3DXMATRIX& pView, D3DXMATRIX& pProjection)
//{
//	D3DXMATRIX mTranslation;
//	D3DXMatrixTranslation(&mTranslation, pEyePos.x,pEyePos.y, pEyePos.z);
//	// Set Input Assembler params
//	mDevice->IASetPrimitiveTopology( D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
//	mShader->SetMatrix("g_mWorldViewProjection", mTranslation * pWorld);
//
//	//set Vertex buffer
//	mVertexBuffer->Apply(0);
//	//set Index buffer
//	mIndexBuffer->Apply(0);
//}
//
//void SkyBox::render()
//{
//	// Render line using the technique g_pRenderTextured
//	D3D10_TECHNIQUE_DESC techDesc;
//	mShader->GetTechnique()->GetDesc( &techDesc );
//	
//	for( unsigned int p = 0; p < techDesc.Passes; p++ )
//	{
//		mShader->Apply(p);
//		mDevice->DrawIndexed( mNumIndices, 0, 0);
//	}
//}

void SkyBox::BuildGeoSphere(unsigned int p_NumSubDivisions, float p_Radius)
{
	//Put a cap on the number of subdivisions
	p_NumSubDivisions = std::min(p_NumSubDivisions, unsigned int(5));

	//Aprox a asphere by tesselating an icosahedron
	const float X = 0.525731f; 
	const float Z = 0.850651f;

	vec3 pos[12] = 
	{
		vec3(-X, 0.0f, Z),  vec3(X, 0.0f, Z),  
		vec3(-X, 0.0f, -Z), vec3(X, 0.0f, -Z),    
		vec3(0.0f, Z, X),   vec3(0.0f, Z, -X), 
		vec3(0.0f, -Z, X),  vec3(0.0f, -Z, -X),    
		vec3(Z, X, 0.0f),   vec3(-Z, X, 0.0f), 
		vec3(Z, -X, 0.0f),  vec3(-Z, -X, 0.0f)
	};

	unsigned int k[60] = 
	{
		1,4,0,  4,9,0,  4,5,9,  8,5,4,  1,8,4,    
		1,10,8, 10,3,8, 8,3,5,  3,2,5,  3,7,2,    
		3,10,7, 10,6,7, 6,11,7, 6,0,11, 6,1,0, 
		10,1,6, 11,0,9, 2,11,9, 5,2,9,  11,2,7 
	};

	m_vertices.resize(12);
	m_initIndices.resize(60);

	for(unsigned int i = 0; i < m_vertices.capacity(); i++)
	{
		m_vertices[i] = pos[i];
	}
	for(unsigned int i = 0; i < m_initIndices.capacity(); i++)
	{
		m_initIndices[i] = k[i];
	}
	for(unsigned int i = 0; i < p_NumSubDivisions; i++)
	{
		Subdivide();	
	}

	// Project m_vertices onto sphere and scale.
	for(size_t i = 0; i < m_vertices.size(); ++i)
	{
		m_vertices[i] = normalize(m_vertices[i]);
		//D3DXVec3Normalize(&m_vertices[i], &m_vertices[i]);
		m_vertices[i] *= p_Radius;
	}
}

void SkyBox::Subdivide()
{
	vector<vec3> vin = m_vertices;
	vector<unsigned int>  iin = m_initIndices;

	m_vertices.resize(0);
	m_initIndices.resize(0);

	//       v1
	//       *
	//      / \
	//     /   \
	//  m0*-----*m1
	//   / \   / \
	//  /   \ /   \
	// *-----*-----*
	// v0    m2     v2

	unsigned int numTris = (unsigned int)iin.size()/3;
	for(unsigned int i = 0; i < numTris; ++i)
	{
		vec3 v0 = vin[ iin[i*3+0] ];
		vec3 v1 = vin[ iin[i*3+1] ];
		vec3 v2 = vin[ iin[i*3+2] ];

		vec3 m0 = 0.5f*(v0 + v1);
		vec3 m1 = 0.5f*(v1 + v2);
		vec3 m2 = 0.5f*(v0 + v2);

		m_vertices.push_back(v0); // 0
		m_vertices.push_back(v1); // 1
		m_vertices.push_back(v2); // 2
		m_vertices.push_back(m0); // 3
		m_vertices.push_back(m1); // 4
		m_vertices.push_back(m2); // 5
 
		m_initIndices.push_back(i*6+0);
		m_initIndices.push_back(i*6+3);
		m_initIndices.push_back(i*6+5);

		m_initIndices.push_back(i*6+3);
		m_initIndices.push_back(i*6+4);
		m_initIndices.push_back(i*6+5);

		m_initIndices.push_back(i*6+5);
		m_initIndices.push_back(i*6+4);
		m_initIndices.push_back(i*6+2);

		m_initIndices.push_back(i*6+3);
		m_initIndices.push_back(i*6+1);
		m_initIndices.push_back(i*6+4);
	}
}

vector<vec3> SkyBox::getVertices()
{
	return m_vertices;
}
vector<unsigned int> SkyBox::getIndices()
{
	return m_initIndices;
}
vec3* SkyBox::getVertexData()
{
  vec3* temp = NULL;
  unsigned int size = m_vertices.size();
  temp				= new vec3[size];
  for(unsigned int i = 0; i < size; i++)
  {
	  temp[i] = m_vertices.at(i);
  }
  return temp;
}
unsigned int* SkyBox::getIndicesData()
{
  unsigned int* temp = NULL;
  unsigned int size = m_initIndices.size();
  temp				= new unsigned int[size];
  for(unsigned int i = 0; i < size; i++)
  {
	  temp[i] = m_initIndices.at(i);
  }
  return temp;
}
//ID3D10ShaderResourceView* SkyBox::getSkySRV()
//{
//	return mRV;
//}