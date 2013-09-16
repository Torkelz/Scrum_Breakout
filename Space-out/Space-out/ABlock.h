#ifndef ABLOCK_H
#define ABLOCK_H

#include "Object.h"

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

	const static int	m_sizeX = 3;
	const static int	m_sizeY = 1;
	const static int	m_sizeZ = 1;

protected:
	int m_hp;
	int m_blockID;
	BlockVertex m_vertex;
};


#endif