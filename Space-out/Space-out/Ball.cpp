#include "Ball.h"

Ball::Ball(vec3* p_pPos, vec3* p_pColor, std::string p_objectName):Object(p_pPos, p_pColor, p_objectName)
{
	m_texturePath	= "sphere.jpg";
	m_boundingVolume = new Sphere(2.5f * 0.5f, *p_pPos);
}

Ball::~Ball(){}

void Ball::update()
{
	updatePosition();
	m_boundingVolume->updatePosition(m_pos);
}

void Ball::updatePosition()
{
	m_pos += m_speed;
}

void Ball::setSpeed(vec3 p_speed)
{
	m_speed = p_speed;
}

vec3 Ball::getSpeed()
{
	return m_speed;
}

std::string* Ball::getTexturePath()
{
	return &m_texturePath;
}