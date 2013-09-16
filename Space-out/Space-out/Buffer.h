#ifndef BUFFER_H
#define BUFFER_H

#include <d3d11.h>

enum BUFFER_TYPE
{
	VERTEX_BUFFER,
	INDEX_BUFFER,
	CONSTANT_BUFFER_VS,
	CONSTANT_BUFFER_GS,
	CONSTANT_BUFFER_PS,
	BUFFER_TYPE_COUNT,
	ALL
};

enum BUFFER_USAGE
{
	BUFFER_DEFAULT,
	BUFFER_STREAM_OUT_TARGET,
	BUFFER_CPU_WRITE,
	BUFFER_CPU_WRITE_DISCARD,
	BUFFER_CPU_READ,
	BUFFER_USAGE_COUNT,
	BUFFER_USAGE_IMMUTABLE //ADDED
};

struct BufferInitDesc
{
	BUFFER_TYPE		type;
	UINT32			numElements;
	UINT32			elementSize;
	BUFFER_USAGE	usage;
	void*			initData;

	BufferInitDesc()
	{
		initData = NULL;
	}
};


class Buffer
{
private:
	ID3D11Buffer*	m_pBuffer;
	BUFFER_TYPE		m_type;
	BUFFER_USAGE	m_usage;

	UINT32			m_elementSize;
	UINT32			m_elementCount;

	ID3D11Device*			m_pDevice;
	ID3D11DeviceContext*	m_pDeviceContext;

	D3D11_MAPPED_SUBRESOURCE m_mappedResource;

public:
	Buffer();
	~Buffer();

	HRESULT apply(UINT32 p_misc = 0);

	HRESULT init(ID3D11Device* p_pDevice, ID3D11DeviceContext* p_pDeviceContext, BufferInitDesc& p_refInitDesc);

	void* map();
	void unmap();

	D3D11_MAPPED_SUBRESOURCE getMappedResource();

	ID3D11Buffer* getBufferPointer();
	void setBuffer(ID3D11Buffer* p_pBuffer);
	UINT32 getVertexSize();
	UINT32 getElementCount();

};

#endif