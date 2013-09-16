#include "Buffer.h"

Buffer::Buffer(){}
Buffer::~Buffer(){}

HRESULT Buffer::apply(UINT32 p_misc)
{
	HRESULT hr = S_OK;

	switch(m_type)
	{
	case VERTEX_BUFFER:
		{
			UINT32 vertexSize = m_elementSize;
			UINT32 offset = 0;
			m_pDeviceContext->IASetVertexBuffers(p_misc, 1, &m_pBuffer, &vertexSize, &offset );
		}
		break;
	case INDEX_BUFFER:
		{
			m_pDeviceContext->IASetIndexBuffer(m_pBuffer, DXGI_FORMAT_R32_UINT, 0);
		}
		break;
	case CONSTANT_BUFFER_VS:
		{
			m_pDeviceContext->VSSetConstantBuffers(p_misc, 1, &m_pBuffer);
		}
		break;
	case CONSTANT_BUFFER_GS:
		{
			m_pDeviceContext->GSSetConstantBuffers(p_misc, 1, &m_pBuffer);
		}
		break;
	case CONSTANT_BUFFER_PS:
		{
			m_pDeviceContext->PSSetConstantBuffers(p_misc, 1, &m_pBuffer);
		}
		break;
	case ALL:
	{
		m_pDeviceContext->VSSetConstantBuffers(p_misc, 1, &m_pBuffer);
		m_pDeviceContext->GSSetConstantBuffers(p_misc, 1, &m_pBuffer);
		m_pDeviceContext->PSSetConstantBuffers(p_misc, 1, &m_pBuffer);
	}
	break;
	default:
		hr = E_FAIL;
		break;
	};

	return hr;
}

HRESULT Buffer::init(ID3D11Device* p_pDevice, ID3D11DeviceContext* p_pDeviceContext, BufferInitDesc& p_refInitDesc)
{
	m_pDevice = p_pDevice;
	m_pDeviceContext = p_pDeviceContext;
	
	D3D11_BUFFER_DESC bufferDesc;

	//TEST
	bufferDesc.StructureByteStride = 0;

	m_type = p_refInitDesc.type;
	switch(m_type)
	{
		case VERTEX_BUFFER:
			{
				bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

				if(p_refInitDesc.usage == BUFFER_STREAM_OUT_TARGET)
					bufferDesc.BindFlags |= D3D11_BIND_STREAM_OUTPUT;
			}
			break;
		case INDEX_BUFFER:
			{
				bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
			}
			break;
		case CONSTANT_BUFFER_VS:
		case CONSTANT_BUFFER_GS:
		case CONSTANT_BUFFER_PS:
		case ALL:
			{
				bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
			}
			break;
		default:
			return E_FAIL;
			break;
	};

	m_usage = p_refInitDesc.usage;
	m_elementSize = p_refInitDesc.elementSize;
	m_elementCount = p_refInitDesc.numElements;

	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;

	if(m_usage == BUFFER_CPU_READ)
	{
		bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
		bufferDesc.CPUAccessFlags |= D3D11_CPU_ACCESS_READ;
	}
	else if(m_usage == BUFFER_CPU_WRITE)
	{
		bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
		bufferDesc.CPUAccessFlags |= D3D11_CPU_ACCESS_WRITE;
	}
	else if(m_usage == BUFFER_CPU_WRITE_DISCARD)
	{
		bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
		bufferDesc.CPUAccessFlags |= D3D11_CPU_ACCESS_WRITE;
	}
	else if(m_usage == BUFFER_USAGE_IMMUTABLE )
	{
		bufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
		bufferDesc.CPUAccessFlags = 0;
	}

	//Desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.MiscFlags = 0;
	bufferDesc.ByteWidth = p_refInitDesc.numElements * p_refInitDesc.elementSize;

	//set at least 16 bytes
	if(bufferDesc.ByteWidth < 16)
		bufferDesc.ByteWidth = 16;

	HRESULT hr = S_OK;
	if(p_refInitDesc.initData)
	{
		D3D11_SUBRESOURCE_DATA data;
		data.pSysMem = p_refInitDesc.initData;
		hr = m_pDevice->CreateBuffer(&bufferDesc, &data, &m_pBuffer);
	}
	else
	{
		hr = m_pDevice->CreateBuffer(&bufferDesc, NULL, &m_pBuffer);
	}

	if(FAILED(hr))
	{
		MessageBox(NULL, "Unable to create buffer.", "Slenda Error", MB_ICONERROR | MB_OK);
	}

	return hr;
}

void* Buffer::map()
{
	void* ret = NULL;
	if(m_usage == BUFFER_CPU_WRITE || m_usage == BUFFER_CPU_READ || m_usage == BUFFER_CPU_WRITE_DISCARD)
	{
		
		UINT32 mapType = 0;

		if(m_usage == BUFFER_CPU_READ)	mapType = D3D11_MAP_READ;
		else if(m_usage == BUFFER_CPU_WRITE) mapType = D3D11_MAP_WRITE;
		else if(m_usage == BUFFER_CPU_WRITE_DISCARD)	mapType = D3D11_MAP_WRITE_DISCARD;

		HRESULT hr = S_OK;
		if( FAILED( hr = m_pDeviceContext->Map(
												m_pBuffer,
												0,
												(D3D11_MAP)mapType,
												0,
												&m_mappedResource ) ) )
		{
			ret = NULL;
		}
		else
		{
			ret = m_mappedResource.pData;
		}
	}

	return ret;
}

void Buffer::unmap()
{
	m_pDeviceContext->Unmap( m_pBuffer, 0 );
}

D3D11_MAPPED_SUBRESOURCE Buffer::getMappedResource()
{
	return m_mappedResource;
}

ID3D11Buffer* Buffer::getBufferPointer()
{
	return m_pBuffer;
}

void Buffer::setBuffer(ID3D11Buffer* p_pBuffer)
{
	m_pBuffer = p_pBuffer;
}

UINT32 Buffer::getVertexSize()
{
	return m_elementSize;
}

UINT32 Buffer::getElementCount()
{
	return m_elementCount;
}