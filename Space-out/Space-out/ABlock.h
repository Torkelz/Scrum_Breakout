#ifndef ABLOCK_H
#define ABLOCK_H

#include "Object.h"
#include "AABB.h"
#include <DirectXMath.h>
#include <gtc\matrix_transform.hpp>

using namespace DirectX;

enum BLOCKTYPE
{
	BLOCK,
	EXPBLOCK,
	HARDBLOCK
};

struct BlockVertex
{
	vec3 pos;
	unsigned int blockType;
};

struct cBlockBuffer
{
	XMMATRIX WVP;
	XMMATRIX rotation;
	float sizeX;
	float sizeY;
	float sizeZ;
};

const vec3 g_bvSize(3.2f, 3.7f, 3.0f);

class ABlock : public Object
{
public:
	ABlock(vec3* p_pPos, vec3* p_pColor, unsigned int p_blockType, std::string p_objectName, vec2 p_blockID);
	~ABlock();
	
	virtual void		init(mat4* p_pRotMat);
	virtual void		update();
	void				setPos(vec3 p_pos, mat4* p_pRotMat);
	// m_hp -= p_hp, OBS minus.
	void				decreaseHP(int p_hp);
	bool				isDead();

	int					getHp();
	vec2				getBlockID();
	BlockVertex			getBlockVertex();
	unsigned int		getBlockType();
	void				changeBlockType(unsigned int p_blockType);
	


protected:
	int m_hp;
	vec2 m_blockID;
	BlockVertex m_vertex;
};


#endif