#include "PowerUp.h"

PowerUp::PowerUp(vec3* p_pPos, vec3* p_pColor, std::string p_objectName) : Object(p_pPos, p_pColor, p_objectName)
{
	m_vertices.push_back(vec3(-2.0f, -2.0f, 0.0f));
	m_vertices.push_back(vec3(2.0f, -2.0f, 0.0f));
	m_vertices.push_back(vec3(-2.0f, 2.0f, 0.0f));
	m_vertices.push_back(vec3(2.0f, 2.0f, 0.0f));
	m_boundingVolume = new AABB(m_vertices.at(3), m_vertices.at(0), vec4(1.0f, 1.0f, 1.0f, 1.0f));
	m_speed = vec3(0.0f, -10.0f, 0.0f);
}

PowerUp::~PowerUp(){}

void PowerUp::update(float p_dt)
{
	updatePosition(p_dt);
	m_boundingVolume->updatePosition(m_pos);
}

void PowerUp::updatePosition(float p_dt)
{
	m_pos += p_dt * m_speed;
}

int PowerUp::getType()
{
	return m_type;
}