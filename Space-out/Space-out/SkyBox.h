#ifndef SKYBOX__H
#define SKYBOX__H

#include "stdafx.h"
#include "Buffer.h"
#include "Shader.h"

class SkyBox
{
public:
	SkyBox(ID3D10Device* pDevice);
	~SkyBox();

	HRESULT init(float pRadius);
	void prepareToRender(D3DXMATRIX& pWorld, D3DXVECTOR3 pEyePos);//, D3DXMATRIX& pView, D3DXMATRIX& pProjection);
	void render();

	ID3D10ShaderResourceView* getSkySRV();

private:
	struct SkyVertex
	{
		D3DXVECTOR3 pos;
	};
	void BuildGeoSphere( UINT pNumSubDivisions, float pRadius,  vector<D3DXVECTOR3>& pVertices, vector<DWORD>& pIndices);
	void Subdivide(vector<D3DXVECTOR3>& pVertices, vector<DWORD>& pIndices);

	ID3D10Device* mDevice;

	Buffer* mVertexBuffer;
	Buffer* mIndexBuffer;
	Shader* mShader;

	ID3D10ShaderResourceView* mRV;

	UINT mNumIndices;
};
#endif;