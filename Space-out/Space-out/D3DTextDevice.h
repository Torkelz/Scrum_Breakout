#pragma once

#include "D3DFont.h"
#include "Shader.h"
#include "Buffer.h"

using namespace DirectX;

class D3DTextDevice
{
private:
	struct SentenceType
	{
		ID3D11Buffer *vertexBuffer, *indexBuffer;
		int vertexCount, indexCount, maxLength;
		float red, green, blue;
		int id;
	};

	struct VertexType
	{
		XMFLOAT3 position;
	    XMFLOAT2 texture;
	};

	struct FontCB
	{
		XMMATRIX worldMatrix;
		XMMATRIX viewMatrix;
		XMMATRIX projectionMatrix;
		XMFLOAT4 color;
	};

public:
	D3DTextDevice();
	~D3DTextDevice();

	bool Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext, HWND hwnd, int screenWidth, int screenHeight);
	void Shutdown();
	bool Render(ID3D11DeviceContext*, XMMATRIX*, XMMATRIX*, ID3D11SamplerState*, ID3D11RasterizerState*, ID3D11BlendState* p_Blend);
	void addSentence(char* message, int id, ID3D11Device*, ID3D11DeviceContext*);
	void removeSentence(int id);
	void updateSentenceAt(int id, char* newMessage, int, int, float, float, float, ID3D11DeviceContext*);
private:
	bool InitializeSentence(SentenceType**, int, ID3D11Device*);
	bool UpdateSentence(SentenceType*, char*, int, int, float, float, float, ID3D11DeviceContext*);
	void ReleaseSentence(SentenceType**);
	bool RenderSentence(ID3D11DeviceContext*, SentenceType*, XMMATRIX*, XMMATRIX*, ID3D11SamplerState*, ID3D11RasterizerState*, ID3D11BlendState* p_Blend);

private:
	D3DFont*		m_Font;
	Shader*			m_FontShader;
	int				m_screenWidth, m_screenHeight;
	XMMATRIX		m_baseViewMatrix;
	Buffer*			m_cBuffer;
	FontCB			m_cb;
	
	std::vector<SentenceType*>	m_sentence;
};