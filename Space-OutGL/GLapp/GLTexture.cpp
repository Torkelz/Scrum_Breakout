#include "Header/GLTexture.h"

GLTexture::GLTexture()//ID3D11Device* p_pDevice, ID3D11DeviceContext* p_pDeviceContext)
{
//	m_pDevice			= p_pDevice;
//	m_pDeviceContext	= p_pDeviceContext;
//	m_pResource			= NULL;
//	m_pResourceView		= NULL;
//	CoInitializeEx(NULL, COINIT_MULTITHREADED);
}

GLTexture::~GLTexture()
{
	release();
}

void GLTexture::release()
{
	//ReleaseCOM(m_pResource);
	//ReleaseCOM(m_pResourceView);
}

//ID3D11Texture2D* D3DTexture::getResource()
//{
//	return m_pResource;
//}
//
//ID3D11ShaderResourceView* D3DTexture::getResourceView()
//{
//	return m_pResourceView;
//}

void GLTexture::bindTextureResource(GLuint p_program, char* p_pUniform, GLuint p_textureID)
{
	GLint texUnitLoc = glGetUniformLocation(p_program, p_pUniform);
	glProgramUniform1i(p_program, texUnitLoc, 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, p_textureID);
}

void GLTexture::createTexture(char* p_pTexturePath, GLuint p_textureID)
{
	FREE_IMAGE_FORMAT format = FreeImage_GetFileType(p_pTexturePath,0);
	FIBITMAP* imagen = FreeImage_Load(format, p_pTexturePath);

	FIBITMAP* temp = imagen;
	imagen = FreeImage_ConvertTo32Bits(imagen);
	FreeImage_Unload(temp);

	int w = FreeImage_GetWidth(imagen);
	int h = FreeImage_GetHeight(imagen);
	GLubyte* texture = new GLubyte[4*w*h];
	char* pixels = (char*)FreeImage_GetBits(imagen);

	for(int j = 0; j<w*h; j++)
	{
		texture[j*4+0]= pixels[j*4+2];
		texture[j*4+1]= pixels[j*4+1];
		texture[j*4+2]= pixels[j*4+0];
		texture[j*4+3]= pixels[j*4+3];
	}
	glGenTextures(1, &p_textureID);
	glBindTexture(GL_TEXTURE_2D, p_textureID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
}

//ID3D11ShaderResourceView* D3DTexture::createTextureSRV(ID3D11Texture2D* p_pTexture)
//{
//	ID3D11ShaderResourceView* pSRV = NULL;
//
//	D3D11_TEXTURE2D_DESC td;
//	p_pTexture->GetDesc(&td);
//
//	//init view description
//	D3D11_SHADER_RESOURCE_VIEW_DESC viewDesc;
//	ZeroMemory( &viewDesc, sizeof(viewDesc) );
//
//	viewDesc.Format					= td.Format;
//	viewDesc.ViewDimension			= D3D11_SRV_DIMENSION_TEXTURE2D;
//	viewDesc.Texture2D.MipLevels	= td.MipLevels;
//
//	if(FAILED(m_pDevice->CreateShaderResourceView(p_pTexture, &viewDesc, &pSRV)))
//	{
//		MessageBox(0, "Unable to create shader resource view", "Error!", 0);
//	}
//
//	return pSRV;
//}
