#include "DXShader.h"

DXShader::DXShader()
:	m_pDevice(0),	 m_pDeviceContext(0),	 m_pInputLayout(0),	 m_pHullShader(0),	 m_pDomainShader(0),	 m_pGeometryShader(0),	 m_pPixelShader(0),	 m_pVertexShader(0)
{
}

DXShader::~DXShader()
{	
}

HRESULT DXShader::InitShader(ID3D11Device* p_pDevice, ID3D11DeviceContext* p_pDeviceContext, char* p_pFilename,
		char* pVSEntryPoint, char* pPSEntryPoint , char* pVShaderModel, char* pPShaderModel,
		const D3D11_INPUT_ELEMENT_DESC* p_pVertexLayout, unsigned int p_NumElements)
{
	HRESULT hr = S_OK;

	m_pDevice = p_pDevice;
	m_pDeviceContext = p_pDeviceContext;
	m_NumElements = p_NumElements;

	CompileAndCreateShaderFromFile( p_pFilename, pVSEntryPoint, pVShaderModel, VERTEX_SHADER, p_pVertexLayout);
	CompileAndCreateShaderFromFile( p_pFilename, pPSEntryPoint, pPShaderModel, PIXEL_SHADER, NULL);
	
	m_pDeviceContext->IASetInputLayout( m_pVertexLayout );
	return hr;
}

void DXShader::CreateHullShader( char* p_pFileName, char* p_pEntryPoint, char* p_pShaderModel )
{
	CompileAndCreateShaderFromFile( p_pFileName, p_pEntryPoint, p_pShaderModel, HULL_SHADER, NULL);
}

void DXShader::CreateDomainShader( char* p_pFileName, char* p_pEntryPoint, char* p_pShaderModel )
{
	CompileAndCreateShaderFromFile( p_pFileName, p_pEntryPoint, p_pShaderModel, DOMAIN_SHADER, NULL);
}

HRESULT DXShader::CompileAndCreateShaderFromFile( char* p_pFileName, char* p_pEntryPoint, char* p_pShaderModel, ShaderType p_pShaderType, const D3D11_INPUT_ELEMENT_DESC* p_pVertexLayout)
{
	HRESULT hr = S_OK;
	DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;

	ID3DBlob* pErrBlob;
	ID3DBlob* pBlob;

	hr = D3DCompileFromFile( (LPCWSTR)p_pFileName,
							 NULL,
							 NULL,
							 p_pEntryPoint,
							 p_pShaderModel,
							 dwShaderFlags,
							 NULL,
							 &pBlob,
							 &pErrBlob);
	if(FAILED(hr))
	{
		MessageBox(0, (char*)pErrBlob->GetBufferPointer(), "Shader Error", 0);

		ReleaseCOM(pErrBlob);
		return hr;
	}

	switch(p_pShaderType)
	{
		case VERTEX_SHADER:
			{
				hr = m_pDevice->CreateVertexShader(		pBlob->GetBufferPointer(),
														pBlob->GetBufferSize(),
														NULL,
														&m_pVertexShader		);

				hr = m_pDevice->CreateInputLayout(		p_pVertexLayout,
														m_NumElements,
														pBlob->GetBufferPointer(),
														pBlob->GetBufferSize(),
														&m_pVertexLayout		);
				break;
			}
		case PIXEL_SHADER:
			{
				hr = m_pDevice->CreatePixelShader(		pBlob->GetBufferPointer(),
														pBlob->GetBufferSize(),
														NULL,
														&m_pPixelShader			);
				break;
			}
		case HULL_SHADER:
			{
				hr = m_pDevice->CreateHullShader(		pBlob->GetBufferPointer(),
														pBlob->GetBufferSize(),
														NULL,
														&m_pHullShader			);
				break;
			}
		case DOMAIN_SHADER:
			{
				hr = m_pDevice->CreateDomainShader(		pBlob->GetBufferPointer(),
														pBlob->GetBufferSize(),
														NULL,
														&m_pDomainShader		);
				break;
			}
		case GEOMETRY_SHADER:
			{
				hr = m_pDevice->CreateGeometryShader(	pBlob->GetBufferPointer(),
														pBlob->GetBufferSize(),
														NULL,
														&m_pGeometryShader		);
				break;
			}
	}
	if(FAILED(hr))
	{
		MessageBox(0, "Error creating shader", "Shader Error!", 0);
	}
	ReleaseCOM( pBlob );
	ReleaseCOM( pErrBlob );

	return hr;
}