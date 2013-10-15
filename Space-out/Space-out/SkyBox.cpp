#include "SkyBox.h"


SkyBox::SkyBox(ID3D10Device* pDevice)
{
	mDevice = pDevice;
	mNumIndices = 0;
}

SkyBox::~SkyBox()
{
	mVertexBuffer->~Buffer();
	mVertexBuffer = NULL;
	mIndexBuffer->~Buffer();
	mIndexBuffer = NULL;
	mShader->~Shader();
	mShader = NULL;
}

HRESULT SkyBox::init(float pRadius)
{
	vector<D3DXVECTOR3> vertices;
	vector<DWORD> initIndices;

	BuildGeoSphere( 2, pRadius, vertices, initIndices );

	vector<SkyVertex> initSkyVertices(vertices.size());
	for(UINT i = 0; i < vertices.size(); i++)
	{
		initSkyVertices[i].pos = 0.5f * vertices[i];
	}

	//Create Vertex Buffer Description
	BUFFER_INIT_DESC bdVertex;
	bdVertex.ElementSize = sizeof(SkyVertex);
	bdVertex.InitData = initSkyVertices.data();
	bdVertex.NumElements = initSkyVertices.size();
	bdVertex.Type = VERTEX_BUFFER;
	bdVertex.Usage = BUFFER_DEFAULT; //BUFFER_DEFAULT
	
	//Create Buffer
	mVertexBuffer = new Buffer();
	if(FAILED(mVertexBuffer->Init(mDevice, bdVertex)))
	{
		return E_FAIL;
	}

	mNumIndices = (UINT)initIndices.size();

	//Index Buffer Description
	BUFFER_INIT_DESC bdIndex;
	bdIndex.ElementSize = sizeof( DWORD );
	bdIndex.InitData = initIndices.data();
	bdIndex.NumElements = mNumIndices;
	bdIndex.Type = INDEX_BUFFER;
	bdIndex.Usage = BUFFER_DEFAULT; //BUFFER_DEFAULT
	
	//Create Index Buffer
	mIndexBuffer = new Buffer();
	if(FAILED(mIndexBuffer->Init(mDevice, bdIndex)))
	{
		return E_FAIL;
	}

	// Define our vertex data layout
	const D3D10_INPUT_ELEMENT_DESC VertexLayout[] =
	{
		{ "POS", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D10_INPUT_PER_VERTEX_DATA, 0 }
	};

	//Create Shader
	mShader = new Shader();
	if(FAILED(mShader->Init(mDevice, "SkyBox.fx", VertexLayout, sizeof(VertexLayout) / sizeof(D3D10_INPUT_ELEMENT_DESC), "RenderSky",
		D3D10_SHADER_ENABLE_BACKWARDS_COMPATIBILITY)))
	{
		return E_FAIL;
	}

	//Load Cube Map
	D3DX10_IMAGE_LOAD_INFO loadInfo;
	loadInfo.MiscFlags = D3D10_RESOURCE_MISC_TEXTURECUBE;

	ID3D10Texture2D* tex = 0;

	if(FAILED(D3DX10CreateTextureFromFile(mDevice, "files\\textures\\grassenvmap1024.dds",
	&loadInfo, 0, (ID3D10Resource**)&tex, 0)))
	{
		return E_FAIL;
	}

	//Send to .fx
	D3D10_TEXTURE2D_DESC texDesc;
	tex->GetDesc(&texDesc);

	D3D10_SHADER_RESOURCE_VIEW_DESC viewDesc;
	viewDesc.Format = texDesc.Format;
	viewDesc.ViewDimension = D3D10_SRV_DIMENSION_TEXTURECUBE;
	viewDesc.TextureCube.MipLevels = texDesc.MipLevels;
	viewDesc.TextureCube.MostDetailedMip = 0;

	
	if(FAILED(mDevice->CreateShaderResourceView(tex, &viewDesc, &mRV)))
	{
		return E_FAIL;
	}
	mShader->SetResource("gCubeMap", mRV);
	SAFE_RELEASE(tex);

	return S_OK;
}

void SkyBox::prepareToRender(D3DXMATRIX& pWorld, D3DXVECTOR3 pEyePos)//, D3DXMATRIX& pView, D3DXMATRIX& pProjection)
{
	D3DXMATRIX mTranslation;
	D3DXMatrixTranslation(&mTranslation, pEyePos.x,pEyePos.y, pEyePos.z);
	// Set Input Assembler params
	mDevice->IASetPrimitiveTopology( D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	mShader->SetMatrix("g_mWorldViewProjection", mTranslation * pWorld);

	//set Vertex buffer
	mVertexBuffer->Apply(0);
	//set Index buffer
	mIndexBuffer->Apply(0);
}

void SkyBox::render()
{
	// Render line using the technique g_pRenderTextured
	D3D10_TECHNIQUE_DESC techDesc;
	mShader->GetTechnique()->GetDesc( &techDesc );
	
	for( UINT p = 0; p < techDesc.Passes; p++ )
	{
		mShader->Apply(p);
		mDevice->DrawIndexed( mNumIndices, 0, 0);
	}
}

void SkyBox::BuildGeoSphere(UINT pNumSubDivisions, float pRadius,  vector<D3DXVECTOR3>& pVertices, vector<DWORD>& pIndices)
{
	//Put a cap on the number of subdivisions
	pNumSubDivisions = min(pNumSubDivisions, UINT(5));

	//Aprox a asphere by tesselating an icosahedron
	const float X = 0.525731f; 
	const float Z = 0.850651f;

	D3DXVECTOR3 pos[12] = 
	{
		D3DXVECTOR3(-X, 0.0f, Z),  D3DXVECTOR3(X, 0.0f, Z),  
		D3DXVECTOR3(-X, 0.0f, -Z), D3DXVECTOR3(X, 0.0f, -Z),    
		D3DXVECTOR3(0.0f, Z, X),   D3DXVECTOR3(0.0f, Z, -X), 
		D3DXVECTOR3(0.0f, -Z, X),  D3DXVECTOR3(0.0f, -Z, -X),    
		D3DXVECTOR3(Z, X, 0.0f),   D3DXVECTOR3(-Z, X, 0.0f), 
		D3DXVECTOR3(Z, -X, 0.0f),  D3DXVECTOR3(-Z, -X, 0.0f)
	};

	DWORD k[60] = 
	{
		1,4,0,  4,9,0,  4,5,9,  8,5,4,  1,8,4,    
		1,10,8, 10,3,8, 8,3,5,  3,2,5,  3,7,2,    
		3,10,7, 10,6,7, 6,11,7, 6,0,11, 6,1,0, 
		10,1,6, 11,0,9, 2,11,9, 5,2,9,  11,2,7 
	};

	pVertices.resize(12);
	pIndices.resize(60);

	for(UINT i = 0; i < pVertices.capacity(); i++)
	{
		pVertices[i] = pos[i];
	}
	for(UINT i = 0; i < pIndices.capacity(); i++)
	{
		pIndices[i] = k[i];
	}
	for(UINT i = 0; i < pNumSubDivisions; i++)
	{
		Subdivide(pVertices, pIndices);	
	}

	// Project vertices onto sphere and scale.
	for(size_t i = 0; i < pVertices.size(); ++i)
	{
		D3DXVec3Normalize(&pVertices[i], &pVertices[i]);
		pVertices[i] *= pRadius;
	}
}

void SkyBox::Subdivide(vector<D3DXVECTOR3>& pVertices, vector<DWORD>& pIndices)
{
	vector<D3DXVECTOR3> vin = pVertices;
	vector<DWORD>  iin = pIndices;

	pVertices.resize(0);
	pIndices.resize(0);

	//       v1
	//       *
	//      / \
	//     /   \
	//  m0*-----*m1
	//   / \   / \
	//  /   \ /   \
	// *-----*-----*
	// v0    m2     v2

	UINT numTris = (UINT)iin.size()/3;
	for(UINT i = 0; i < numTris; ++i)
	{
		D3DXVECTOR3 v0 = vin[ iin[i*3+0] ];
		D3DXVECTOR3 v1 = vin[ iin[i*3+1] ];
		D3DXVECTOR3 v2 = vin[ iin[i*3+2] ];

		D3DXVECTOR3 m0 = 0.5f*(v0 + v1);
		D3DXVECTOR3 m1 = 0.5f*(v1 + v2);
		D3DXVECTOR3 m2 = 0.5f*(v0 + v2);

		pVertices.push_back(v0); // 0
		pVertices.push_back(v1); // 1
		pVertices.push_back(v2); // 2
		pVertices.push_back(m0); // 3
		pVertices.push_back(m1); // 4
		pVertices.push_back(m2); // 5
 
		pIndices.push_back(i*6+0);
		pIndices.push_back(i*6+3);
		pIndices.push_back(i*6+5);

		pIndices.push_back(i*6+3);
		pIndices.push_back(i*6+4);
		pIndices.push_back(i*6+5);

		pIndices.push_back(i*6+5);
		pIndices.push_back(i*6+4);
		pIndices.push_back(i*6+2);

		pIndices.push_back(i*6+3);
		pIndices.push_back(i*6+1);
		pIndices.push_back(i*6+4);
	}
}

ID3D10ShaderResourceView* SkyBox::getSkySRV()
{
	return mRV;
}