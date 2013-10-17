#ifndef SHADER_H_
#define SHADER_H_

#include "../../GLapp/Header/GLUtil.h"

enum ShaderType
{
	VERTEX_SHADER,
	FRAGMENT_SHADER,
	GEOMETRY_SHADER
};

class Shader
{
public:
	Shader();
	~Shader();

	bool init();
	bool createAndCompile(ShaderType p_shaderType, const char* p_pFileAddress);
	void attachAndLink();
	// ## applying ##
	void apply();
	void uniformBlockBinding(GLuint p_bindingPoint, const char* p_pUniformName);
	GLuint getProgram();

private:
	bool errorCheckShader(GLuint p_pShader, const char* p_pStrShaderType);

private:
	GLuint		m_program;
	GLuint		m_vertex;
	GLuint		m_fragment;
	GLuint		m_geometry;
};


#endif /* SHADER_H_ */




