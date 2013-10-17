#pragma once

#include "Object.h"
#include "AABB.h"
#include <gtc\matrix_transform.hpp>

enum PowerUpType
{
	FASTERBALL = 0,
	SLOWERBALL,
	BIGGERPAD,
	SMALLERPAD,
	STICKYPAD
};

class PowerUp : public Object
{
protected:
	vec3			m_speed;
	PowerUpType		m_type;
	mat4			m_scale;
public:
	PowerUp(vec3* p_pPos, vec3* p_pColor, std::string p_objectName);
	~PowerUp();

	void	update(float p_dt, mat4 p_translate);
	void	updatePosition(float p_dt);
	int		getType();
	bool	collide(BoundingVolume* p_pVolume);
	mat4	getScale();
};

