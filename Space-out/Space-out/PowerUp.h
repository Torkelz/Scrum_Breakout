#pragma once

#include "Object.h"
#include "AABB.h"

enum PowerUpType
{
	FASTERBALL = 0,
	SLOWERBALL,
	BIGGERPAD,
	SMALLERPAD,
	STICKYPAD,
	EXPLOSIVEBALL,
	POWERUPTYPECOUNT // if adding new poweruptype, keep this at the bottom.
};

class PowerUp : public Object
{
protected:
	vec3			m_speed;
	PowerUpType		m_type;
public:
	PowerUp(vec3* p_pPos, vec3* p_pColor, std::string p_objectName);
	~PowerUp();

	void	update(float p_dt, mat4 p_translate);
	void	updatePosition(float p_dt);
	int		getType();
	bool	collide(BoundingVolume* p_pVolume);
};

