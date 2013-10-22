#include "Header/PUScatterBall.h"


PUScatterBall::PUScatterBall(vec3* p_pPos, vec3* p_pColor, std::string p_objectName) : PowerUp(p_pPos, p_pColor, p_objectName)
{
	m_texturePath = new std::wstring(L"Picatures/scatterBall.png");
	m_type = SCATTERBALL;
}


PUScatterBall::~PUScatterBall()
{
}
