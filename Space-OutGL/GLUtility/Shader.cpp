#include "Header/Shader.h"
#include <fstream>

Shader::Shader()
{
	m_program = 0;
	m_vertex = 0;
	m_fragment = 0;
	m_geometry = 0;
}

Shader::~Shader()
{

}

bool Shader::init()
{
	return true;
}

bool Shader::createAndCompile(ShaderType p_shaderType, const char* p_pFilecAddress)
{
	std::string shaderCode;
	std::ifstream shaderStream(p_pFilecAddress,std::ios::in);
	if(shaderStream.is_open())
	{
		std::string Line = "";
		while(getline(shaderStream, Line))
			shaderCode += "\n" + Line;
		shaderStream.close();
	}

	const char* sourceVersion = shaderCode.c_str();
	GLint lengthVersion = shaderCode.length();

	switch (p_shaderType)
	{
	case VERTEX_SHADER:
		m_vertex = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(m_vertex,1, &sourceVersion, &lengthVersion);
		glCompileShader(m_vertex);
		if(!errorCheckShader(m_vertex, "VertexShader"))
		{
			return false;
		}
		break;
	case FRAGMENT_SHADER:
		m_fragment = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(m_fragment,1, &sourceVersion, &lengthVersion);
		glCompileShader(m_fragment);
		if(!errorCheckShader(m_fragment, "FragmentShader"))
		{
			return false;
		}
		break;
	case GEOMETRY_SHADER:
		m_geometry = glCreateShader(GL_GEOMETRY_SHADER);
		glShaderSource(m_geometry,1, &sourceVersion, &lengthVersion);
		glCompileShader(m_geometry);
		if(!errorCheckShader(m_geometry, "GeometryShader"))
		{
			return false;
		}
		break;
	default:
		break;
	}

	m_program = glCreateProgram();

	return true;
}

void Shader::attachAndLink()
{
	if(m_vertex != 0)
		glAttachShader(m_program, m_vertex);
	if(m_geometry != 0)
		glAttachShader(m_program, m_geometry);
	if(m_fragment != 0)
		glAttachShader(m_program, m_fragment);

	glLinkProgram(m_program);

	GLint checkErr;
	glGetProgramiv(m_program, GL_LINK_STATUS, &checkErr);
	if(checkErr == GL_FALSE)
	{
		fprintf(stderr, "Program link failure \n");
	}
}

void Shader::apply()
{
	glUseProgram(m_program);
}

bool Shader::errorCheckShader(GLuint p_pShader, const char* p_pStrShaderType)
{
	GLint status;
	glGetShaderiv(p_pShader, GL_COMPILE_STATUS, &status);
	if (status == GL_FALSE)
	{
		GLint infoLogLength;
		glGetShaderiv(p_pShader, GL_INFO_LOG_LENGTH, &infoLogLength);

		GLchar *strInfoLog = new GLchar[infoLogLength + 1];
		glGetShaderInfoLog(p_pShader, infoLogLength, NULL, strInfoLog);

		fprintf(stderr, "Compile failure in %s shader:\n%s\n", p_pStrShaderType, strInfoLog);
		delete[] strInfoLog;
		return false;
	}
	return true;
}

void Shader::uniformBlockBinding(GLuint p_bindingPoint, const char* p_pUniformName)
{
	GLuint blockIndex = glGetUniformBlockIndex(m_program, p_pUniformName);
	glUniformBlockBinding(m_program, blockIndex, p_bindingPoint);
}

//void shader::uniformMatrix4fv();

GLint Shader::getProgram()
{
	return m_program;
}
