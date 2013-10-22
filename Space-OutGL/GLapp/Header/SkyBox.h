#ifndef SKYBOX__H
#define SKYBOX__H

//#include "stdafx.h"
//#include "Buffer.h"
//#include "Shader.h"
#include <glm/glm.hpp>
#include <vector>

using namespace glm;
using namespace std;

class SkyBox
{
public:
	SkyBox();//ID3D10Device* pDevice);
	~SkyBox();

	bool init(float p_Radius);
	//void prepareToRender(D3DXMATRIX& pWorld, D3DXVECTOR3 pEyePos);//, D3DXMATRIX& pView, D3DXMATRIX& pProjection);
	//void render();

	//ID3D10ShaderResourceView* getSkySRV();
	vector<vec3> getVertices();
	vector<unsigned int> getIndices();
	vec3* getVertexData();

	unsigned int* getIndicesData();

	/*struct SkyVertex
	{
		vec3 pos;
	};*/
private:
	
	void BuildGeoSphere( unsigned int p_NumSubDivisions, float p_Radius);
	void Subdivide();

	//ID3D10Device* mDevice;

	//Buffer* mVertexBuffer;
	//Buffer* mIndexBuffer;
	//Shader* mShader;

	//ID3D10ShaderResourceView* mRV;

	vector<vec3> m_vertices;
	vector<unsigned int> m_initIndices;

	unsigned int m_NumIndices;
};
#endif;
