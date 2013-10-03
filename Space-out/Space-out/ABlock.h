#ifndef ABLOCK_H
#define ABLOCK_H

#include "Object.h"
#include "AABB.h"
#include <DirectXMath.h>

using namespace DirectX;

struct BlockVertex
{
	vec3 pos;
	vec4 color;
};

struct cBlockBuffer
{
	XMMATRIX WVP;
	XMMATRIX rotation;
	float sizeX;
	float sizeY;
	float sizeZ;
};

const vec3 g_bvSize(3.0f, 3.0f, 1.0f);

class ABlock : public Object
{
public:
	ABlock(vec3* p_pPos, vec3* p_pColor, std::string p_objectName, vec2 p_blockID);
	~ABlock();
	
	virtual void		init(mat4* p_pRotMat);
	virtual void		update();
	void				setPos(vec3 p_pos, mat4* p_pRotMat);
	int					getHp();
	vec2				getBlockID();
	BlockVertex			getBlockVertex();
	

protected:
	int m_hp;
	vec2 m_blockID;
	BlockVertex m_vertex;

};


#endif