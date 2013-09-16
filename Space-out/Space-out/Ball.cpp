#include "Ball.h"

Ball::Ball(Vector3* p_pPos, Vector3* p_pColor, std::string p_objectName):Object(p_pPos, p_pColor, p_objectName)
{
	m_texturePath	= "sphere.jpg";
}

Ball::~Ball(){}

std::string* Ball::getTexturePath()
{
	return &m_texturePath;
}