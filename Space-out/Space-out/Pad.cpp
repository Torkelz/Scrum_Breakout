#include "Pad.h"

Pad::Pad(Vector3* p_pPos, Vector3* p_pColor, std::string p_objectName) : Object(p_pPos, p_pColor, p_objectName)
{
	m_vertices.push_back(Vector3(0.0f, 0.0f, 0.0f));
	m_vertices.push_back(Vector3(0.0f, 10.0f, 0.0f));
	m_vertices.push_back(Vector3(10.0f, 0.0f, 0.0f));
	m_vertices.push_back(Vector3(10.0f, 10.0f, 0.0f));
}

Pad::~Pad(){}

void Pad::setPos(Vector2 p_pos)
{
	m_pos.x = p_pos.x;
}