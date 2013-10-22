#include "Header/PUSlowerBall.h"


PUSlowerBall::PUSlowerBall(vec3* p_pos, vec3* p_color, std::string p_objectName) : PowerUp(p_pos, p_color, p_objectName)
{
	m_texturePath = new std::wstring(L"Picatures/slow.png");
	m_type = SLOWERBALL;
}

PUSlowerBall::~PUSlowerBall(){}
