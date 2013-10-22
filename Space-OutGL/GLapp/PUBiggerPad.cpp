#include "Header/PUBiggerPad.h"


PUBiggerPad::PUBiggerPad(vec3* p_pos, vec3* p_color, std::string p_objectName) : PowerUp(p_pos, p_color, p_objectName)
{
	m_texturePath = new std::wstring(L"Picatures/fastAndFurious7.png");
	m_type = BIGGERPAD;
}

PUBiggerPad::~PUBiggerPad(){}
