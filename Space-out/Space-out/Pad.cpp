#include "Pad.h"

Pad::Pad(Vector3* p_pos, Vector3* p_color, std::string p_objectName) : Object(p_pos, p_color, p_objectName)
{
	vertices.push_back(Vector3(0.0f, 0.0f, 0.0f));
	vertices.push_back(Vector3(0.0f, 10.0f, 0.0f));
	vertices.push_back(Vector3(10.0f, 0.0f, 0.0f));
	vertices.push_back(Vector3(10.0f, 10.0f, 0.0f));
}

Pad::~Pad(){}

void Pad::setPos(Vector2 p_pos)
{
	m_pos.x = p_pos.x;
}