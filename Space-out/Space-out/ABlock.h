#ifndef ABLOCK_H
#define ABLOCK_H

#include "Object.h"
#include <DirectXMath.h>

using namespace DirectX;

struct BlockVertex
{
	Vector3 pos;
	Vector4 color;
};

struct cBlockBuffer
{
	XMMATRIX WVP;
	float sizeX;
	float sizeY;
	float sizeZ;
};

const static float	g_bSizeX = 3.0f;
const static float 	g_bSizeY = 1.0f;
const static float	g_bSizeZ = 1.0f;

class ABlock : public Object
{
public:
	ABlock(Vector3* p_pPos, Vector3* p_pColor, std::string p_objectName, int p_blockID);
	~ABlock();
	
	virtual void		init();
	virtual void		update();
	void				setPos(Vector2 p_pos);
	int					getHp();
	int					getBlockID();
	BlockVertex			getBlockVertex();

protected:
	int m_hp;
	int m_blockID;
	BlockVertex m_vertex;
};


#endif