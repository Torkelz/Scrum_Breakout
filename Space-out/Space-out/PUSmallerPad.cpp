#include "PUSmallerPad.h"

PUSmallerPad::PUSmallerPad(vec3* p_pPos, vec3* p_pColor, std::string p_objectName) : PowerUp(p_pPos, p_pColor, p_objectName)
{
	m_texturePath = new std::wstring(L"Picatures/smallerPad.png");
	m_type = SMALLERPAD;
}

PUSmallerPad::~PUSmallerPad(){}