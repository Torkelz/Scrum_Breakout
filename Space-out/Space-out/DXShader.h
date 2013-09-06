#ifndef DXSHADER_H
#define DXSHADER_H

#include "d3dApp.h"
#include <d3dcompiler.h>
#include <D3D11Shader.h>

enum ShaderType
{
	VERTEX_SHADER,
	HULL_SHADER,
	DOMAIN_SHADER,
	GEOMETRY_SHADER,
	PIXEL_SHADER
};

class DXShader
{
private:
	ID3D11Device*			m_pDevice;
	ID3D11DeviceContext*	m_pDeviceContext;
	ID3D11InputLayout*		m_pInputLayout;
	ID3D11HullShader*		m_pHullShader;
	ID3D11DomainShader*		m_pDomainShader;
	ID3D11GeometryShader*	m_pGeometryShader;
	ID3D11PixelShader*		m_pPixelShader;
	ID3D11VertexShader*		m_pVertexShader;
	ID3D11InputLayout*		m_pVertexLayout;
	ShaderType				m_ShaderType;
	UINT					m_NumElements;

public:	
	DXShader();
	~DXShader();

	HRESULT InitShader(ID3D11Device* p_pDevice, ID3D11DeviceContext* p_pDeviceContext, char* p_pFilename,
		char* pVSEntryPoint, char* pPSEntryPoint , char* pVShaderModel, char* pPShaderModel,
		const D3D11_INPUT_ELEMENT_DESC* p_pVertyexLayout, unsigned int p_NumElements);

	HRESULT CompileAndCreateShaderFromFile(char* pFileName, char* pEntryPoint, char* pShaderModel, 
		ShaderType sType, const D3D11_INPUT_ELEMENT_DESC* pVertexLayout );
	
	void CreateHullShader( char* pFileName, char* pEntryPoint, char* pShaderModel );

	void CreateDomainShader( char* pFileName, char* pEntryPoint, char* pShaderModel );
	
	//set/get Functions

};


#endif	//DXSHADER_H