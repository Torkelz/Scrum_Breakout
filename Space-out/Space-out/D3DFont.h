#pragma once

#include <fstream>

#include "d3dUtil.h"
#include "D3DTexture.h"
using namespace DirectX;

class D3DFont
{
private:
	struct FontType
	{
		float left, right;
		int size;
	};

	struct VertexType
	{
		XMFLOAT3 position;
		XMFLOAT2 texture;
	};

public:
	D3DFont(void);
	~D3DFont(void);

	bool Initialize(ID3D11Device*, ID3D11DeviceContext*, char*, std::wstring*);
	void Shutdown();

	ID3D11ShaderResourceView* GetTexture();

	void BuildVertexArray(void*, char*, float, float);

private:
	bool LoadFontData(char*);
	void ReleaseFontData();
	bool LoadTexture(ID3D11Device*, ID3D11DeviceContext*, std::wstring*);
	void ReleaseTexture();

private:
	FontType* m_Font;
	D3DTexture* m_Texture;
};
