#include "Header/PUSplitBall.h"


PUSplitBall::PUSplitBall(vec3* p_pPos, vec3* p_pColor, std::string p_objectName) : PowerUp(p_pPos, p_pColor, p_objectName)
{
	m_texturePath = new std::wstring(L"Picatures/splitBall.png");
	m_type = SPLITBALL;
}


PUSplitBall::~PUSplitBall()
{
}
