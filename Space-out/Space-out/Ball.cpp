#include "Ball.h"

Ball::Ball(vec3* p_pPos, vec3* p_pColor, std::string p_objectName):Object(p_pPos, p_pColor, p_objectName)
{
	m_texturePath = new std::wstring(L"Picatures/sphere.png");
	m_internPosition = vec2(*p_pPos);
	
	m_speed = vec3(5.0f, -10.0f, 0.0f);
	m_stuck = false;
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

void Ball::setSpeed(vec3 p_speed)
{
	m_speed = p_speed;
}

vec3 Ball::getSpeed()
{
	return m_speed;
}

void Ball::speedUp()
{
	m_speed *= 1.5f;
}

void Ball::speedDown()
{
	m_speed *= 0.75f;
}

void Ball::setStuck(bool p_stuck)
{
	m_stuck = p_stuck;
}

bool Ball::getStuck()
{
	return m_stuck;
}

void Ball::setInternalPosition(vec3 p_position, vec3 p_origPos, vec3 p_right, vec3 p_down)
{
	vec2 t;
	vec3 temp;
	temp = p_position - p_origPos;
	t.x = abs(dot(temp,p_right));
	t.y = abs(dot(temp,p_down));
	m_internPosition = t;
}

vec3 Ball::getRealPosition()
{
	return m_realPosition;
}
