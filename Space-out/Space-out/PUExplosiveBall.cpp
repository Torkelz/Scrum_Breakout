#include "PUExplosiveBall.h"

PUExplosiveBall::PUExplosiveBall(vec3* p_pPos, vec3* p_pColor, std::string p_objectName) : PowerUp(p_pPos, p_pColor, p_objectName)
{
	m_texturePath = new std::wstring(L"Picatures/explosiveBall.png");
	m_type = EXPLOSIVEBALL;
}

PUExplosiveBall::~PUExplosiveBall(){}