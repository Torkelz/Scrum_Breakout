#include "Shader.h"

Shader::Shader(): m_pDevice(0), m_pDeviceContext(0), m_pVertexLayout(0), m_pHullShader(0), m_pDomainShader(0), m_pGeometryShader(0), m_pPixelShader(0), m_pVertexShader(0){}
Shader::~Shader(){}

void Shader::init(ID3D11Device* p_pDevice, ID3D11DeviceContext* p_pDeviceContext, unsigned int p_NumElements)
{
	m_pDevice = p_pDevice;
	m_pDeviceContext = p_pDeviceContext;
	m_NumElements = p_NumElements;
}

HRESULT Shader::compileAndCreateShaderFromFile( LPCWSTR p_pFileName, char* p_pEntryPoint, char* p_pShaderModel, ShaderType p_pShaderType, const D3D11_INPUT_ELEMENT_DESC* p_pVertexLayout)
{
	HRESULT hr = S_OK;
	DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
	dwShaderFlags |= D3DCOMPILE_DEBUG;

	ID3DBlob* pErrBlob;
	ID3DBlob* pBlob;

	hr = D3DCompileFromFile( p_pFileName,
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
		case GEOMETRY_SHADER:
		{
			hr = m_pDevice->CreateGeometryShader(	pBlob->GetBufferPointer(),
													pBlob->GetBufferSize(),
													NULL,
													&m_pGeometryShader		);
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
	}
	if(FAILED(hr))
	{
		MessageBox(0, "Error creating shader", "Shader Error!", 0);
	}
	ReleaseCOM( pBlob );
	ReleaseCOM( pErrBlob );

	return hr;
}

void Shader::setShaders()
{
	if(m_pVertexShader)
	{
		m_pDeviceContext->IASetInputLayout(m_pVertexLayout);
		m_pDeviceContext->VSSetShader(m_pVertexShader,0,0);
	}
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

void Shader::setResource(ShaderType p_pShaderType,UINT p_StartSpot,UINT p_NumViews, ID3D11ShaderResourceView* p_pShaderResource)
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
		case GEOMETRY_SHADER:
		{
			m_pDeviceContext->GSSetShaderResources(p_StartSpot, p_NumViews, &p_pShaderResource);
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
	}
}