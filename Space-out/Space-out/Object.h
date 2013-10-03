#ifndef OBJECT_H
#define OBJECT_H

#include <string>
#include <vector>
#include "BoundingVolume.h"
#include <stdint.h>

class Object
{
protected:
	std::string		m_objectName;
	vec3			m_pos;
	vec3			m_color;
	float			m_size;
	BoundingVolume*	m_boundingVolume;
	int				m_textureID;
	std::vector<vec3> m_vertices;
	std::wstring*	m_texturePath;
public:
	Object(vec3* p_pPos, vec3* p_pColor, std::string p_objectName);
	~Object();

	void					update();

	void					setPos(vec3 p_pos);
	vec3*					getPos();
	std::vector<vec3>*		getVertices();
	BoundingVolume*			getBoundingVolume();
	std::wstring*			getTexturePath();
};

#endif OBJECT_H