#include "ABlock.h"

ABlock::ABlock(Vector3* p_pPos, Vector3* p_pColor, std::string p_objectName, int p_blockID) : Object(p_pPos, p_pColor, p_objectName)
{
	m_hp = 0;
	m_blockID = p_blockID;
	m_vertex.pos = *p_pPos;
	m_vertex.color = Vector4(p_pColor->x, p_pColor->y, p_pColor->z, 1.0f);
}

ABlock::~ABlock(){}

void ABlock::setPos(Vector2 p_pos)
{
	m_pos.x = p_pos.x;
}

int ABlock::getHp()
{
	return m_hp;
}

int ABlock::getBlockID()
{
	return m_blockID;
}

BlockVertex ABlock::getBlockVertex()
{
	return m_vertex;
}

void ABlock::init(){}

void ABlock::update(){}
