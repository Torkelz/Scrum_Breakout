#pragma once

#include "D3DFont.h"
#include "Shader.h"

using namespace DirectX;

class D3DTextDevice
{
private:
	struct SentenceType
	{
		ID3D11Buffer *vertexBuffer, *indexBuffer;
		int vertexCount, indexCount, maxLength;
		float red, green, blue;
	};

	struct VertexType
	{
		XMFLOAT3 position;
	    XMFLOAT2 texture;
	};

public:
	D3DTextDevice();
	~D3DTextDevice();

	bool Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext, HWND hwnd, int screenWidth, int screenHeight, XMMATRIX* baseViewMatrix);
	void Shutdown();
	bool Render(ID3D11DeviceContext*, XMMATRIX*, XMMATRIX*);

private:
	bool InitializeSentence(SentenceType**, int, ID3D11Device*);
	bool UpdateSentence(SentenceType*, char*, int, int, float, float, float, ID3D11DeviceContext*);
	void ReleaseSentence(SentenceType**);
	bool RenderSentence(ID3D11DeviceContext*, SentenceType*, XMMATRIX*, XMMATRIX*);

private:
	D3DFont*		m_Font;
	Shader*			m_FontShader;
	int				m_screenWidth, m_screenHeight;
	XMMATRIX		m_baseViewMatrix;
	SentenceType*	m_sentence1;
	SentenceType*	m_sentence2;
};