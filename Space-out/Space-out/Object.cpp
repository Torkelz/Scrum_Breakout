#include "Object.h"

Object::Object(vec3* p_pos, vec3* p_color, std::string p_objectName)
{
	m_pos			= *p_pos;
	m_color			= *p_color;
	m_objectName	= p_objectName;
}

Object::~Object(){}

void Object::update(){}

vec3* Object::getPos()
{
	return &m_pos;
}

std::vector<vec3>* Object::getVertices()
{
	return &m_vertices;
}

BoundingVolume* Object::getBoundingVolume()
{
	return &m_boundingVolume;
}