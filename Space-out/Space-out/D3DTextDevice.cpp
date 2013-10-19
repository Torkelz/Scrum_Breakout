#include "D3DTextDevice.h"

D3DTextDevice::D3DTextDevice()
{
	m_Font = 0;
	m_FontShader = 0;
}

D3DTextDevice::~D3DTextDevice(){}


bool D3DTextDevice::Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext, HWND hwnd, int screenWidth, int screenHeight)
{
	bool result;

	m_cBuffer = new Buffer();

	BufferInitDesc cbBallDesc;
	cbBallDesc.elementSize = sizeof(FontCB);
	cbBallDesc.initData = NULL;
	cbBallDesc.numElements = 1;
	cbBallDesc.type = CONSTANT_BUFFER_ALL;
	cbBallDesc.usage = BUFFER_DEFAULT;
	
	m_cBuffer->init(device, deviceContext, cbBallDesc);
	deviceContext->UpdateSubresource(m_cBuffer->getBufferPointer(), 0, NULL, &m_cb, 0, 0);
	m_cBuffer->apply(0);

	// Store the screen width and height.
	m_screenWidth = screenWidth;
	m_screenHeight = screenHeight;

	// Create the font object.
	m_Font = new D3DFont();
	if(!m_Font)
	{
		return false;
	}

	// Initialize the font object.
	result = m_Font->Initialize(device, deviceContext, "Font/fontdata.txt", new std::wstring(L"Font/font.dds"));
	if(!result)
	{
		//MessageBox(hwnd, L"Could not initialize the font object.", L"Error", MB_OK);
		return false;
	}

	// Create the font shader object.
	m_FontShader = new Shader();
	if(!m_FontShader)
	{
		return false;
	}

	D3D11_INPUT_ELEMENT_DESC desc[] = 
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TEXTUREPOSITION", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};

	// Initialize the font shader object.
	m_FontShader->init(device, deviceContext, 2);
	m_FontShader->compileAndCreateShaderFromFile(L"Font.fx", "FontVertexShader", "vs_5_0", VERTEX_SHADER, desc);
	m_FontShader->compileAndCreateShaderFromFile(L"Font.fx", "FontPixelShader", "ps_5_0", PIXEL_SHADER, NULL);

	return true;
}

void D3DTextDevice::Shutdown()
{
	// Release the font object.
	if(m_Font)
	{
		m_Font->Shutdown();
		delete m_Font;
		m_Font = 0;
	}

	return;
}

bool D3DTextDevice::Render(ID3D11DeviceContext* deviceContext, XMMATRIX* worldMatrix, XMMATRIX* orthoMatrix, ID3D11SamplerState* pSS, ID3D11RasterizerState* pRS)
{
	for (int i = 0; i < m_sentence.size(); i++)
	{
		RenderSentence(deviceContext, m_sentence.at(i), worldMatrix, orthoMatrix, pSS, pRS);
	}

	return true;
}

bool D3DTextDevice::InitializeSentence(SentenceType** sentence, int maxLength, ID3D11Device* device)
{
	VertexType* vertices;
	unsigned long* indices;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
    D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;
	int i;

	// Create a new sentence object.
	*sentence = new SentenceType;
	if(!*sentence)
	{
		return false;
	}

	// Initialize the sentence buffers to null.
	(*sentence)->vertexBuffer = 0;
	(*sentence)->indexBuffer = 0;

	// Set the maximum length of the sentence.
	(*sentence)->maxLength = maxLength;

	// Set the number of vertices in the vertex array.
	(*sentence)->vertexCount = 6 * maxLength;

	// Set the number of indexes in the index array.
	(*sentence)->indexCount = (*sentence)->vertexCount;

	// Create the vertex array.
	vertices = new VertexType[(*sentence)->vertexCount];
	if(!vertices)
	{
		return false;
	}

	// Create the index array.
	indices = new unsigned long[(*sentence)->indexCount];
	if(!indices)
	{
		return false;
	}

	// Initialize vertex array to zeros at first.
	memset(vertices, 0, (sizeof(VertexType) * (*sentence)->vertexCount));

	// Initialize the index array.
	for(i=0; i<(*sentence)->indexCount; i++)
	{
		indices[i] = i;
	}

	// Set up the description of the dynamic vertex buffer.
    vertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
    vertexBufferDesc.ByteWidth = sizeof(VertexType) * (*sentence)->vertexCount;
    vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    vertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the vertex data.
    vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	// Create the vertex buffer.
    result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &(*sentence)->vertexBuffer);
	if(FAILED(result))
	{
		return false;
	}

	// Set up the description of the static index buffer.
    indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    indexBufferDesc.ByteWidth = sizeof(unsigned long) * (*sentence)->indexCount;
    indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
    indexBufferDesc.CPUAccessFlags = 0;
    indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the index data.
    indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	// Create the index buffer.
	result = device->CreateBuffer(&indexBufferDesc, &indexData, &(*sentence)->indexBuffer);
	if(FAILED(result))
	{
		return false;
	}

	// Release the vertex array as it is no longer needed.
	delete [] vertices;
	vertices = 0;

	// Release the index array as it is no longer needed.
	delete [] indices;
	indices = 0;

	return true;
}

bool D3DTextDevice::UpdateSentence(SentenceType* sentence, char* text, int positionX, int positionY, float red, float green, float blue,
							   ID3D11DeviceContext* deviceContext)
{
	int numLetters;
	VertexType* vertices;
	float drawX, drawY;
	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	VertexType* verticesPtr;


	// Store the color of the sentence.
	sentence->red = red;
	sentence->green = green;
	sentence->blue = blue;

	// Get the number of letters in the sentence.
	numLetters = (int)strlen(text);

	// Check for possible buffer overflow.
	if(numLetters > sentence->maxLength)
	{
		return false;
	}

	// Create the vertex array.
	vertices = new VertexType[sentence->vertexCount];
	if(!vertices)
	{
		return false;
	}

	// Initialize vertex array to zeros at first.
	memset(vertices, 0, (sizeof(VertexType) * sentence->vertexCount));

	// Calculate the X and Y pixel position on the screen to start drawing to.
	drawX = (float)(((m_screenWidth / 2) * -1) + positionX);
	drawY = (float)((m_screenHeight / 2) - positionY);

	// Use the font class to build the vertex array from the sentence text and sentence draw location.
	//m_Font->BuildVertexArray((void*)vertices, text, drawX, drawY);
	m_Font->BuildVertexArray((void*)vertices, text, drawX, drawY);

	// Lock the vertex buffer so it can be written to.
	result = deviceContext->Map(sentence->vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if(FAILED(result))
	{
		return false;
	}

	// Get a pointer to the data in the vertex buffer.
	verticesPtr = (VertexType*)mappedResource.pData;

	// Copy the data into the vertex buffer.
	memcpy(verticesPtr, (void*)vertices, (sizeof(VertexType) * sentence->vertexCount));

	// Unlock the vertex buffer.
	deviceContext->Unmap(sentence->vertexBuffer, 0);

	// Release the vertex array as it is no longer needed.
	delete [] vertices;
	vertices = 0;

	return true;
}

void D3DTextDevice::ReleaseSentence(SentenceType** sentence)
{
	if(*sentence)
	{
		// Release the sentence vertex buffer.
		if((*sentence)->vertexBuffer)
		{
			(*sentence)->vertexBuffer->Release();
			(*sentence)->vertexBuffer = 0;
		}

		// Release the sentence index buffer.
		if((*sentence)->indexBuffer)
		{
			(*sentence)->indexBuffer->Release();
			(*sentence)->indexBuffer = 0;
		}

		// Release the sentence.
		delete *sentence;
		*sentence = 0;
	}

	return;
}


bool D3DTextDevice::RenderSentence(ID3D11DeviceContext* deviceContext, SentenceType* sentence, XMMATRIX* worldMatrix, 
							   XMMATRIX* orthoMatrix, ID3D11SamplerState* pSS, ID3D11RasterizerState* pRS)
{
	unsigned int stride, offset;
	XMFLOAT4 pixelColor;
	bool result;

	// Set vertex buffer stride and offset.
    stride = sizeof(VertexType); 
	offset = 0;

	// Set the vertex buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetVertexBuffers(0, 1, &sentence->vertexBuffer, &stride, &offset);

    // Set the index buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetIndexBuffer(sentence->indexBuffer, DXGI_FORMAT_R32_UINT, 0);

    // Set the type of primitive that should be rendered from this vertex buffer, in this case triangles.
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// Create a pixel color vector with the input sentence color.
	pixelColor = XMFLOAT4(sentence->red, sentence->green, sentence->blue, 1.0f);

	// Render the text using the font shader.

	m_cb.worldMatrix		= *worldMatrix;
	m_cb.projectionMatrix	= *orthoMatrix;
	m_cb.viewMatrix			= XMMatrixIdentity();
	m_cb.color				= pixelColor;

	m_FontShader->setShaders();
	deviceContext->UpdateSubresource(m_cBuffer->getBufferPointer(), 0, NULL, &m_cb, 0, 0);
	m_cBuffer->apply(0);
	m_FontShader->setBlendState(NULL);
	m_FontShader->setResource(PIXEL_SHADER, 0, 1, m_Font->GetTexture());
	m_FontShader->setSamplerState(PIXEL_SHADER, 0, 1, pSS);
	deviceContext->RSSetState(pRS);

	deviceContext->DrawIndexed(sentence->indexCount, 0, 0);

	return true;
}

void D3DTextDevice::addSentence(char* message, int id, ID3D11Device* device, ID3D11DeviceContext* deviceContext)
{
	// Initialize the first sentence.
	SentenceType* tempSentence;
	InitializeSentence(&tempSentence, 70, device);

	// Now update the sentence vertex buffer with the new string information.
	//UpdateSentence(tempSentence, message, 100, 200, 1.0f, 1.0f, 1.0f, deviceContext);
	tempSentence->id = id;

	m_sentence.push_back(tempSentence);
}

void D3DTextDevice::removeSentence(int id)
{
	for (int i = 0; i < m_sentence.size(); i++)
	{
		if(m_sentence.at(i)->id == id)
		{
			m_sentence.erase(m_sentence.begin() + i);
			m_sentence.shrink_to_fit();
		}
	}
}

void D3DTextDevice::updateSentenceAt(int id, char* newMessage, int positionX, int positionY, float red, float green, float blue,
							   ID3D11DeviceContext* deviceContext)
{
	for (int i = 0; i < m_sentence.size(); i++)
	{
		if(m_sentence.at(i)->id == id)
		{
			UpdateSentence(m_sentence.at(i), newMessage, positionX, positionY, red, green, blue, deviceContext);
		}
	}
}