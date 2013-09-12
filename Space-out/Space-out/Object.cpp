#include "Object.h"

Object::Object(Vector3* p_pos, Vector3* p_color, std::string p_objectName)
{
	m_pos			= *p_pos;
	m_color			= *p_color;
	m_objectName	= p_objectName;
}

Object::~Object(){}

Vector3* Object::getPos()
{
	return &m_pos;
}

std::vector<Vector3>* Object::getVertices()
{
	return &vertices;
}

