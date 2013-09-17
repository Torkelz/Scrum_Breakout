#include "Pad.h"

Pad::Pad(vec3* p_pPos, vec3* p_pColor, std::string p_objectName) : Object(p_pPos, p_pColor, p_objectName)
{
	m_vertices.push_back(vec3(0.0f, 0.0f, 0.0f));
	m_vertices.push_back(vec3(0.0f, 10.0f, 0.0f));
	m_vertices.push_back(vec3(10.0f, 0.0f, 0.0f));
	m_vertices.push_back(vec3(10.0f, 10.0f, 0.0f));
	m_boundingVolume = AABB(m_vertices.at(0), m_vertices.at(3), vec4(1.0f, 1.0f, 1.0f, 1.0f));
}

Pad::~Pad(){}

void Pad::update(mat4 p_translate)
{
	m_boundingVolume.updatePosition(mat4(1.0f),p_translate);
}

void Pad::setPos(vec2 p_pos)
{
	m_pos.x = p_pos.x;
}