#ifndef OBJECT_H
#define OBJECT_H

#include <string>
#include <vector>
#include "Vector2.h"

class Object
{
protected:
	std::string		m_objectName;
	Vector3			m_pos;
	Vector3			m_color;
	float			m_size;

	std::vector<Vector3> m_vertices;

public:
	Object(Vector3* p_pPos, Vector3* p_pColor, std::string p_objectName);
	~Object();

	Vector3*				getPos();
	std::vector<Vector3>*	getVertices();

};

#endif OBJECT_H