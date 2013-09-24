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
	float sizeX;
	float sizeY;
	float sizeZ;
};

const vec3 g_bvSize(3.0f, 1.0f, 1.0f);

class ABlock : public Object
{
public:
	ABlock(vec3* p_pPos, vec3* p_pColor, std::string p_objectName, int p_blockID);
	~ABlock();
	
	virtual void		init(mat4* p_pRotMat);
	virtual void		update();
	void				setPos(vec2 p_pos);
	int					getHp();
	int					getBlockID();
	BlockVertex			getBlockVertex();

protected:
	int m_hp;
	int m_blockID;
	BlockVertex m_vertex;

};


#endif