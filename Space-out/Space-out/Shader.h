#ifndef DXSHADER_H
#define DXSHADER_H

#include "d3dApp.h"
#include <d3dcompiler.h>
#include <D3D11Shader.h>
#include <DirectXMath.h>

using namespace DirectX;

enum ShaderType
{
	VERTEX_SHADER,
	PIXEL_SHADER,
	GEOMETRY_SHADER,
	HULL_SHADER,
	DOMAIN_SHADER
};

class Shader
{
private:
	ID3D11Device*			m_pDevice;
	ID3D11DeviceContext*	m_pDeviceContext;
	ID3D11HullShader*		m_pHullShader;
	ID3D11DomainShader*		m_pDomainShader;
	ID3D11GeometryShader*	m_pGeometryShader;
	ID3D11PixelShader*		m_pPixelShader;
	ID3D11VertexShader*		m_pVertexShader;
	ID3D11InputLayout*		m_pVertexLayout;
	ShaderType				m_ShaderType;
	UINT					m_NumElements;

public:	
	Shader();
	~Shader();

	void init(ID3D11Device* p_pDevice, ID3D11DeviceContext* p_pDeviceContext, unsigned int p_NumElements);

	HRESULT compileAndCreateShaderFromFile(LPCWSTR p_pFileName, char* p_pEntryPoint, char* p_pShaderModel, 
		ShaderType p_ShaderType, const D3D11_INPUT_ELEMENT_DESC* p_pVertexLayout );
	
	
	void setShaders();

	void setResource(ShaderType p_pShaderType, UINT p_StartSpot, UINT p_NumViews, ID3D11ShaderResourceView* p_pShaderResource);
};


#endif	//DXSHADER_H