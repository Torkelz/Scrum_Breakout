#include "ABlock.h"

ABlock::ABlock(vec3* p_pPos, vec3* p_pColor, std::string p_objectName, vec2 p_blockID) : Object(p_pPos, p_pColor, p_objectName)
{
	m_hp = 0;
	m_blockID = p_blockID;
	m_vertex.pos = *p_pPos;
	m_vertex.color = vec4(p_pColor->x, p_pColor->y, p_pColor->z, 1.0f);

}

ABlock::~ABlock(){}

void ABlock::setPos(vec3 p_pos)
{
	m_pos = p_pos;
	m_vertex.pos = p_pos;
}

int ABlock::getHp()
{
	return m_hp;
}

vec2 ABlock::getBlockID()
{
	return m_blockID;
}

BlockVertex ABlock::getBlockVertex()
{
	return m_vertex;
}

void ABlock::init(mat4* p_pRotMat)
{
	vec4 temp = *p_pRotMat * vec4(g_bvSize, 0.0f);
	vec3 temp1 = vec3(temp.x, temp.y, temp.z);
	m_boundingVolume = new AABB((m_pos + temp1), (m_pos - temp1), vec4(m_color, 1));
}

void ABlock::update(){}
