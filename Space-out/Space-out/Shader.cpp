#include "Shader.h"

DXShader::Shader()
:	m_pDevice(0),	 m_pDeviceContext(0),	 m_pInputLayout(0),	 m_pHullShader(0),	 m_pDomainShader(0),	 m_pGeometryShader(0),	 m_pPixelShader(0),	 m_pVertexShader(0)
{
}

DXShader::~Shader()
{	
}

HRESULT Shader::InitShader(ID3D11Device* p_pDevice, ID3D11DeviceContext* p_pDeviceContext, char* p_pFilename,
		char* p_pVSEntryPoint, char* p_pPSEntryPoint , char* p_pVShaderModel, char* p_pPShaderModel,
		const D3D11_INPUT_ELEMENT_DESC* p_pVertexLayout, unsigned int p_NumElements)
{
	HRESULT hr = S_OK;

	m_pDevice = p_pDevice;
	m_pDeviceContext = p_pDeviceContext;
	m_NumElements = p_NumElements;

	CompileAndCreateShaderFromFile( p_pFilename, p_pVSEntryPoint, p_pVShaderModel, VERTEX_SHADER, p_pVertexLayout);
	CompileAndCreateShaderFromFile( p_pFilename, p_pPSEntryPoint, p_pPShaderModel, PIXEL_SHADER, NULL);
	
	m_pDeviceContext->IASetInputLayout( m_pVertexLayout );
	return hr;
}

void Shader::CreateHullShader( char* p_pFileName, char* p_pEntryPoint, char* p_pShaderModel )
{
	CompileAndCreateShaderFromFile( p_pFileName, p_pEntryPoint, p_pShaderModel, HULL_SHADER, NULL);
}

void Shader::CreateDomainShader( char* p_pFileName, char* p_pEntryPoint, char* p_pShaderModel )
{
	CompileAndCreateShaderFromFile( p_pFileName, p_pEntryPoint, p_pShaderModel, DOMAIN_SHADER, NULL);
}

HRESULT Shader::CompileAndCreateShaderFromFile( char* p_pFileName, char* p_pEntryPoint, char* p_pShaderModel, ShaderType p_pShaderType, const D3D11_INPUT_ELEMENT_DESC* p_pVertexLayout)
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

void Shader::SetShaders()
{
	if(m_pVertexShader){m_pDeviceContext->VSSetShader(m_pVertexShader,NULL,0);}
	else{m_pDeviceContext->VSSetShader(NULL,NULL,0);}

	if(m_pPixelShader){m_pDeviceContext->PSSetShader(m_pPixelShader,NULL,0);}
	else{m_pDeviceContext->PSSetShader(NULL,NULL,0);}

	if(m_pGeometryShader){m_pDeviceContext->GSSetShader(m_pGeometryShader,NULL,0);}
	else{m_pDeviceContext->GSSetShader(NULL,NULL,0);}

	if(m_pDomainShader){m_pDeviceContext->DSSetShader(m_pDomainShader,NULL,0);}
	else{m_pDeviceContext->DSSetShader(NULL,NULL,0);}

	if(m_pHullShader){m_pDeviceContext->HSSetShader(m_pHullShader,NULL,0);}
	else{m_pDeviceContext->HSSetShader(NULL,NULL,0);}
}

void Shader::SetResource(ShaderType p_pShaderType,UINT p_StartSpot,UINT p_NumViews, ID3D11ShaderResourceView* p_pShaderResource)
{
	switch(p_pShaderType)
	{
		case VERTEX_SHADER:
			{
				m_pDeviceContext->VSSetShaderResources(p_StartSpot, p_NumViews, &p_pShaderResource);
				break;
			}
		case PIXEL_SHADER:
			{
				m_pDeviceContext->PSSetShaderResources(p_StartSpot, p_NumViews, &p_pShaderResource);
				break;
			}
		case HULL_SHADER:
			{
				m_pDeviceContext->HSSetShaderResources(p_StartSpot, p_NumViews, &p_pShaderResource);
				break;
			}
		case DOMAIN_SHADER:
			{
				m_pDeviceContext->DSSetShaderResources(p_StartSpot, p_NumViews, &p_pShaderResource);
				break;
			}
		case GEOMETRY_SHADER:
			{
				m_pDeviceContext->GSSetShaderResources(p_StartSpot, p_NumViews, &p_pShaderResource);
				break;
			}
	}
}