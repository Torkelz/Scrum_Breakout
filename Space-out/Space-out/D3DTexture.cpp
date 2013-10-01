#include "D3DTexture.h"

D3DTexture::D3DTexture(ID3D11Device* p_pDevice, ID3D11DeviceContext* p_pDeviceContext)
{
	m_pDevice			= p_pDevice;
	m_pDeviceContext	= p_pDeviceContext;
	m_pResource			= NULL;
	m_pResourceView		= NULL;
	CoInitializeEx(NULL, COINIT_MULTITHREADED);
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

ID3D11Texture2D* D3DTexture::createTextureResource(DXGI_FORMAT p_dxFormat, UINT p_uWidth, UINT p_uHeight, UINT p_uRowPitch, VOID* p_pInitData)
{
	ID3D11Texture2D* pTexture = NULL;

	D3D11_TEXTURE2D_DESC desc;
	desc.Width = p_uWidth;
	desc.Height = p_uHeight;
	desc.MipLevels = 1;
	desc.ArraySize = 1;
	desc.Format = p_dxFormat;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	desc.CPUAccessFlags = 0;
	desc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA data;
	data.pSysMem = p_pInitData;
	data.SysMemPitch = p_uRowPitch;

	if(FAILED(m_pDevice->CreateTexture2D( &desc, p_pInitData ? &data : NULL, &pTexture )))
	{
		int i = 0;
	}

	return pTexture;
}

void D3DTexture::createTexture(std::wstring* p_pTextureFilename, char* p_pDebugName)
{
	m_pResource = createTextureResource(DXGI_FORMAT_R8G8B8A8_UNORM, 173, 178, 0, NULL);
	const wchar_t* temp = p_pTextureFilename->c_str();
	if(SUCCEEDED(CreateWICTextureFromFile(m_pDevice, m_pDeviceContext, temp, (ID3D11Resource**)m_pResource, &m_pResourceView, NULL)))
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