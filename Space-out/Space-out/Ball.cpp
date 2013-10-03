#include "Ball.h"

Ball::Ball(vec3* p_pPos, vec3* p_pColor, std::string p_objectName):Object(p_pPos, p_pColor, p_objectName)
{
	m_texturePath = new std::wstring(L"Picatures/sphere.png");
	m_boundingVolume = new Sphere(2.5f, vec3(p_pPos->x, p_pPos->y - 30.0f, 50.0f));
	m_speed = vec3(10.0f, -20.0f, 0.0f);
}

Ball::~Ball(){}

void Ball::update(float p_dt)
{
	updatePosition(p_dt);
	m_boundingVolume->updatePosition(vec3(m_pos.x, m_pos.y - 30.0f, 50.0f));
}

void Ball::updatePosition(float p_dt)
{
	m_pos += p_dt * m_speed;
}

void Ball::setSpeed(vec3 p_speed)
{
	m_speed = p_speed;
}

vec3 Ball::getSpeed()
{
	return m_speed;
}
