#include "Block.h"

Block::Block(vec3* p_pPos, vec3* p_pColor, std::string p_objectName, int p_blockID) : ABlock(p_pPos, p_pColor, p_objectName, p_blockID)
{
	m_hp = 1;
}

Block::~Block(){}

void Block::init(mat4* p_pRotMat)
{
	ABlock::init(p_pRotMat);

}

void Block::update()
{
	if(m_hp == 0)
	{
		//Dead, destroy object using BlockID.
	}
	//Collision check?
	
}

