#include "Ball.h"

Ball::Ball(vec3* p_pPos, vec3* p_pColor, std::string p_objectName):Object(p_pPos, p_pColor, p_objectName)
{
	m_texturePath = new std::wstring(L"Picatures/sphere.png");

	m_internPosition = vec2(*p_pPos);
	m_speed = vec3(5.0f, -10.0f, 0.0f);
}

Ball::~Ball(){}

void Ball::init(vec3 p_origPos, vec3 p_right, vec3 p_down)
{
	vec3 bPos = p_origPos;
	bPos += p_right * m_internPosition.x;
	bPos += p_down * m_internPosition.y;
	m_realPosition = bPos;
	m_boundingVolume = new Sphere(3.0f, vec3(bPos.x, bPos.y, bPos.z));
}

void Ball::update(float p_dt)
{
	updatePosition(p_dt);
}

void Ball::updatePosition(float p_dt)
{
	m_internPosition += p_dt * vec2(m_speed);
}

void Ball::updateBoundingVolume(vec3 p_origPos, vec3 p_right, vec3 p_down)
{
	vec3 bPos = p_origPos;
	bPos += p_right * m_internPosition.x;
	bPos += p_down * m_internPosition.y;
	m_realPosition = bPos;
	m_boundingVolume->updatePosition(vec3(bPos.x, bPos.y, bPos.z));
}

void Ball::setPosition(vec3 p_pos)
{
	m_pos = p_pos;
}

void Ball::setSpeed(vec3 p_speed)
{
	m_speed = p_speed;
}

vec3 Ball::getSpeed()
{
	return m_speed;
}

vec3 Ball::getRealPosition()
{
	return m_realPosition;
}