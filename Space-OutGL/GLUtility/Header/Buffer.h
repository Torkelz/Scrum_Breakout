
#ifndef BUFFER_H_
#define BUFFER_H_
#include "../../GLapp/Header/GLUtil.h"

struct BufferInputDesc
{
	GLint size;
	GLenum type;
	GLboolean normalized;
	unsigned int pointer;
};
class Buffer
{
public:
	Buffer();
	~Buffer();
	bool	 		init(
						GLenum p_type, void* p_pData, unsigned int p_sizeof,
						unsigned int p_numElements, GLenum p_usage,
						BufferInputDesc* p_pDesc, unsigned int p_numDesc);
	bool 			bufferAdd(unsigned int p_buffer, GLenum p_pUsage, void* p_pType);
	bool 			apply();
	bool			deApply();
	unsigned int 	getBuffer();
	GLsizei			getNumElem();
private:
	GLuint		 	m_buffer;
	GLenum			m_type;
	unsigned int 	m_numDesc;
	unsigned int	m_sizeof;
	GLsizei			m_numElem;
	BufferInputDesc* m_pDesc;

};


#endif /* BUFFER_H_ */
