#include "Header/GLTexture.h"

GLTexture::GLTexture()//ID3D11Device* p_pDevice, ID3D11DeviceContext* p_pDeviceContext)
{
	//success = IL_TRUE;
}

GLTexture::~GLTexture()
{

}

void GLTexture::init()
{

	//ilInit();
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

void GLTexture::createTexture(std::string p_pTexturePath, unsigned int& p_textureID)
{
	FREE_IMAGE_FORMAT format = FreeImage_GetFileType(p_pTexturePath.c_str(),0);
	FIBITMAP* image = FreeImage_Load(format, p_pTexturePath.c_str());

	FIBITMAP* temp = image;
	image = FreeImage_ConvertTo32Bits(image);
	FreeImage_Unload(temp);

	int w = FreeImage_GetWidth(image);
	int h = FreeImage_GetHeight(image);

	GLubyte* textur = new GLubyte[4*w*h];
	char* pixels = (char*)FreeImage_GetBits(image);
	for(int j = 0; j < w*h; j++)
	{
		textur[j*4+0]= pixels[j*4+2];
		textur[j*4+1]= pixels[j*4+1];
		textur[j*4+2]= pixels[j*4+0];
		textur[j*4+3]= pixels[j*4+3];
	}

	glGenTextures(1, &p_textureID);
	glBindTexture(GL_TEXTURE_2D, p_textureID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, (void*)textur);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

//	ilEnable(IL_ORIGIN_SET);
//	ilOriginFunc(IL_ORIGIN_LOWER_LEFT);
//
//	ILuint ilImageID = ilGenImage();
//	ilBindImage(ilImageID);
//	if (!ilLoadImage(p_pTexturePath.c_str()))
//	{
//		ILenum err = ilGetError();
//		//fprintf(stderr, "%s\n", ilGetString(ilGetError()));
//		int dummy = 42;
//	}
//
//
//	//ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);
//	//ILubyte* data = ilGetData();
//	glGenTextures(1, &p_textureID);
//	glBindTexture(GL_TEXTURE_2D, p_textureID);
//	//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, ilGetInteger(IL_IMAGE_WIDTH), ilGetInteger(IL_IMAGE_HEIGHT), 0, ilGetInteger(IL_IMAGE_FORMAT), ilGetInteger(IL_IMAGE_TYPE), ilGetData());
//	float data[16] = {
//			0.f, 1.f, 1.f, 1.f,
//			0.f, 0.f, 1.f, 1.f,
//			0.f, 1.f, 0.f, 1.f,
//			1.f, 0.f, 0.f, 1.f};
//	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 2, 2, 0, GL_RGBA, GL_FLOAT, data);
//	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//
//	ilDeleteImage(ilImageID);
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