#include "PUBiggerPad.h"

PUBiggerPad::PUBiggerPad(vec3* p_pPos, vec3* p_pColor, std::string p_objectName) : PowerUp(p_pPos, p_pColor, p_objectName)
{
	m_texturePath = new std::wstring(L"Picatures/biggerPad.png");
	m_type = BIGGERPAD;
}

PUBiggerPad::~PUBiggerPad(){}