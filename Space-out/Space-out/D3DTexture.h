#pragma once

//#pragma comment( lib, "dxguid.lib")
#include "d3dUtil.h"
#include "WICTextureLoader.h"

class D3DTexture
{
private:
	ID3D11Texture2D*			m_pResource;
	ID3D11ShaderResourceView*	m_pResourceView;
	ID3D11DeviceContext*        m_pDeviceContext;
	ID3D11Device*				m_pDevice;

	ID3D11ShaderResourceView*	createTextureSRV(ID3D11Texture2D* p_pTexture);
public:
	D3DTexture(){};
	D3DTexture(ID3D11Device* p_pDevice, ID3D11DeviceContext* p_pDeviceContext);
	~D3DTexture();

	void release();

	ID3D11Texture2D*			getResource();
	ID3D11ShaderResourceView*	getResourceView();
	void						createTexture(std::wstring* p_pTextureFilename, char* p_pDebugName, unsigned int p_miscFlags);
	ID3D11Texture2D*			createTextureResource(DXGI_FORMAT p_dxFormat, UINT p_uWidth, UINT p_uHeight, UINT p_uRowPitch, VOID* p_pInitData);
};
