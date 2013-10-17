#ifndef	GLTEXTURES_H_
#define GLTEXTURES_H_
#include "GLUtil.h"
#include "FreeImage.h"

class GLTexture
{
private:
//	ID3D11Texture2D*			m_pResource;
//	ID3D11ShaderResourceView*	m_pResourceView;
//	ID3D11DeviceContext*        m_pDeviceContext;
//	ID3D11Device*				m_pDevice;

//	ID3D11ShaderResourceView*	createTextureSRV(ID3D11Texture2D* p_pTexture);
public:
	GLTexture();
	GLTexture(char* p_pTexturePath, GLuint* p_pTextureID);// p_pDevice, ID3D11DeviceContext* p_pDeviceContext);
	~GLTexture();

	void release();

//	ID3D11Texture2D*			getResource();
//	ID3D11ShaderResourceView*	getResourceView();
	void						bindTextureResource(GLuint p_program, char* p_pUniform, GLuint p_textureID);
	void						createTexture(char* p_pTexturePath, GLuint p_textureID);

};
//http://lwjgl.org/wiki/index.php?title=GLSL_Tutorial:_Texturing
#endif /* GLTEXTURES_H_ */
