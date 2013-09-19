#include "Header/Buffer.h"

Buffer::Buffer()
{
	m_buffer = 0;
	m_type = GL_ARRAY_BUFFER;
	m_numElem = 0;
	m_numDesc = 0;
}

Buffer::~Buffer()
{
	glDeleteBuffers(1, &m_buffer);
}

bool Buffer::init(
					GLenum p_type, void* p_pData, unsigned int p_sizeof,
					unsigned int p_numElements, GLenum p_usage,
					BufferInputDesc p_pDesc[], unsigned int p_numDesc)
{
	unsigned int i;
	m_type = p_type;
	m_numDesc = p_numDesc;
	m_numElem = p_numElements;
	glGenBuffers(1, &m_buffer);
	glBindBuffer(m_type, m_buffer);
	glBufferData(m_type, m_numElem * p_sizeof, &p_pData, p_usage);

	GLsizei stride = 0;
	for(i = 0; i < m_numDesc; i++)
	{
		glEnableVertexAttribArray(i);
		glVertexAttribPointer(	i, p_pDesc[i].size, p_pDesc[i].type,
								p_pDesc[i].normalized, stride,
								p_pDesc[i].pointer);
		stride += p_pDesc[i].size;
	}
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	if(glGetError() == GL_NO_ERROR)
		return true;
	else
		return false;
}

bool Buffer::apply()
{
	unsigned int i;
	glBindBuffer(GL_ARRAY_BUFFER, m_buffer);
	for(i = 0; i < m_numDesc; i++)
	{
		glEnableVertexAttribArray(i);
	}
	if(glGetError() == GL_NO_ERROR)
		return true;
	else
		return false;
}

unsigned int Buffer::getBuffer()
{
	return m_buffer;
}

GLsizei Buffer::getNumElem()
{
	return m_numElem;
}
