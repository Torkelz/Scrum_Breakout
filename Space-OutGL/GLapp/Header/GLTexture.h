#ifndef	GLTEXTURES_H_
#define GLTEXTURES_H_
#include "GLUtil.h"
#include "FreeImage.h"

using namespace std;

class GLTexture
{
public:
	//ILboolean success;

//	ID3D11ShaderResourceView*	createTextureSRV(ID3D11Texture2D* p_pTexture);
public:
	GLTexture();
	GLTexture(char* p_pTexturePath, GLuint* p_pTextureID);// p_pDevice, ID3D11DeviceContext* p_pDeviceContext);
	~GLTexture();

	void init();

//	ID3D11Texture2D*			getResource();
//	ID3D11ShaderResourceView*	getResourceView();
	void						bindTextureResource(GLuint p_program, char* p_pUniform, unsigned int p_textureID);
	void						createTexture(std::string p_pTexturePath, unsigned int& p_textureID);

};
//http://lwjgl.org/wiki/index.php?title=GLSL_Tutorial:_Texturing
#endif /* GLTEXTURES_H_ */