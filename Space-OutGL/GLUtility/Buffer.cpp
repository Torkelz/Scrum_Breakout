#include "Header/Buffer.h"

Buffer::Buffer()
{
	m_buffer = 0;
	m_type = GL_ARRAY_BUFFER;
	m_numElem = 0;
	m_numDesc = 0;
	m_pDesc = 0;
	m_sizeof = 0;
}

Buffer::~Buffer()
{
	glDeleteBuffers(1, &m_buffer);
}

bool Buffer::init(
					GLenum p_type, void* p_pData, unsigned int p_sizeof,
					unsigned int p_numElements, GLenum p_usage,
					BufferInputDesc* p_pDesc, unsigned int p_numDesc)
{
	m_type = p_type;
	m_numDesc = p_numDesc;
	m_pDesc = p_pDesc;
	m_numElem = p_numElements;
	m_sizeof = p_sizeof;
	glGenBuffers(1, &m_buffer);
	glBindBuffer(m_type, m_buffer);
	glBufferData(m_type, m_numElem * m_sizeof, p_pData, p_usage);
	glBindBuffer(m_type, 0);

	if(glGetError() == GL_NO_ERROR)
		return true;
	else
		return false;
}

bool Buffer::apply()
{
	unsigned int i;
	GLsizei stride = 0;

	for(i = 0; i < m_numDesc; i++)
	{
		glEnableVertexAttribArray(i);
	}

	glBindBuffer(m_type, m_buffer);

	for(i = 0; i < m_numDesc; i++)
	{
		glVertexAttribPointer(	i, m_pDesc[i].size, m_pDesc[i].type,
				m_pDesc[i].normalized, m_sizeof,
				(void*)stride);
		stride += m_pDesc[i].pointer;
	}

	if(glGetError() == GL_NO_ERROR)
		return true;
	else
		return false;
}

bool Buffer::deApply()
{
	unsigned int i;
	for(i = 0; i < m_numDesc; i++)
	{
		glDisableVertexAttribArray(i);
	}
	glBindBuffer(m_type, 0);
	if(glGetError() == GL_NO_ERROR)
		return true;
	else
		return false;
}

void Buffer::bindBufferBase(GLuint p_bindingPoint)
{
	glBindBufferBase(m_type, p_bindingPoint, m_buffer);
}

void Buffer::setSubData(GLintptr p_offset, GLsizeiptr p_sizeof, GLvoid* p_data)
{
	glBindBuffer(m_type, m_buffer);
	glBufferSubData(m_type, p_offset, p_sizeof, p_data);
}

unsigned int Buffer::getBuffer()
{
	return m_buffer;
}

GLsizei Buffer::getNumElem()
{
	return m_numElem;
}
