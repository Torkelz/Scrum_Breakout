#include "ABlock.h"

ABlock::ABlock(vec3* p_pPos, vec3* p_pColor, unsigned int p_blockType, std::string p_objectName, vec2 p_blockID) : Object(p_pPos, p_pColor, p_objectName)
{
	
	m_hp = 1;
	m_blockID = p_blockID;
	m_vertex.pos = *p_pPos;
	m_vertex.blockType = p_blockType;
	//m_vertex.color = vec4(p_pColor->x, p_pColor->y, p_pColor->z, 1.0f);
	m_boundingVolume = nullptr;
}

ABlock::~ABlock()
{
	if(m_boundingVolume != nullptr)
	{
		delete m_boundingVolume;
		m_boundingVolume = nullptr;
	}
}

void ABlock::setPos(vec3 p_pos, mat4* p_pRotMat)
{
	m_pos = p_pos;
	m_vertex.pos = p_pos;
	vec4 temp = *p_pRotMat * vec4(g_bvSize, 0.0f);
	vec3 temp1 = vec3(temp.x, temp.y, temp.z);
	if(m_boundingVolume != nullptr)
	{
		delete m_boundingVolume;
		m_boundingVolume = nullptr;
	}
	m_boundingVolume = new AABB((g_bvSize), (- g_bvSize), vec4(m_color, 1));
	//m_boundingVolume = new AABB((temp1), (- temp1), vec4(m_color, 1));
	mat4 trans = translate(mat4(1.0f), m_pos);
	m_boundingVolume->updatePosition(mat4(1.0f),mat4(1.0f), trans);
}

void ABlock::decreaseHP(int p_hp)
{
	m_hp -= p_hp;
}

bool ABlock::isDead()
{
	if(m_hp <= 0)
		return true;
	return false;
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

unsigned int ABlock::getBlockType()
{
	return m_vertex.blockType;
}

void ABlock::changeBlockType(unsigned int p_blockType)
{
	m_vertex.blockType = p_blockType;
}

void ABlock::init(mat4* p_pRotMat)
{
	vec4 temp = *p_pRotMat * vec4(g_bvSize, 0.0f);
	vec3 temp1 = vec3(temp.x, temp.y, temp.z);
	m_boundingVolume = new AABB(temp1, - temp1, vec4(m_color, 1));
}

void ABlock::update(){}
