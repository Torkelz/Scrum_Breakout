#include "HardBlock.h"

HardBlock::HardBlock(vec3* p_pPos, vec3* p_pColor, unsigned int p_blockType, std::string p_objectName, vec2 p_blockID) : ABlock(p_pPos, p_pColor, p_blockType, p_objectName, p_blockID)
{
	m_texturePath = new std::wstring(L"Picatures/expBlock.png");
	m_hp = 3;
}

HardBlock::~HardBlock(){}

void HardBlock::init(mat4* p_pRotMat)
{
	ABlock::init(p_pRotMat);
}

void HardBlock::update()
{
	if(m_hp == 0)
	{
		//Dead, destroy object using BlockID.
	}
	//Collision check?
	
}

