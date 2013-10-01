#include "D3DTexture.h"

D3DTexture::D3DTexture(ID3D11Device* p_pDevice, ID3D11DeviceContext* p_pDeviceContext)
{
	m_pDevice			= p_pDevice;
	m_pDeviceContext	= p_pDeviceContext;
	m_pResource			= NULL;
	m_pResourceView		= NULL;
}

D3DTexture::~D3DTexture()
{
	release();
}

void D3DTexture::release()
{
	ReleaseCOM(m_pResource);
	ReleaseCOM(m_pResourceView);
}

ID3D11Texture2D* D3DTexture::getResource()
{
	return m_pResource;
}

ID3D11ShaderResourceView* D3DTexture::getResourceView()
{
	return m_pResourceView;
}

void D3DTexture::createTexture(wchar_t* p_pTextureFilename, char* p_pDebugName)
{
	if(SUCCEEDED(CreateWICTextureFromFile(m_pDevice, m_pDeviceContext, p_pTextureFilename, (ID3D11Resource**)m_pResource, &m_pResourceView, NULL)))
	{
		m_pResourceView = createTextureSRV(m_pResource);
		
		/*if(p_pDebugName)
		{
			if(m_pResource)			setDebugName(m_pResource, p_pDebugName);
			if(m_pResourceView)		setDebugName(m_pResourceView, p_pDebugName);
		}*/
	}
}

ID3D11ShaderResourceView* D3DTexture::createTextureSRV(ID3D11Texture2D* p_pTexture)
{
	ID3D11ShaderResourceView* pSRV = NULL;

	D3D11_TEXTURE2D_DESC td;
	p_pTexture->GetDesc(&td);

	//init view description
	D3D11_SHADER_RESOURCE_VIEW_DESC viewDesc; 
	ZeroMemory( &viewDesc, sizeof(viewDesc) ); 
	
	viewDesc.Format					= td.Format;
	viewDesc.ViewDimension			= D3D11_SRV_DIMENSION_TEXTURE2D;
	viewDesc.Texture2D.MipLevels	= td.MipLevels;

	if(FAILED(m_pDevice->CreateShaderResourceView(p_pTexture, &viewDesc, &pSRV)))
	{
		MessageBox(0, "Unable to create shader resource view", "Error!", 0);
	}

	return pSRV;
}