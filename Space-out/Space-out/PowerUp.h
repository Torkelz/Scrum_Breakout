#pragma once

#include "Object.h"
#include "AABB.h"

enum PowerUpType
{
	FASTERBALL = 0,
	SLOWERBALL
};

class PowerUp : public Object
{
private:
	vec3			m_speed;
	PowerUpType		m_type;
public:
	PowerUp(vec3* p_pPos, vec3* p_pColor, std::string p_objectName);
	~PowerUp();

	void	update(float p_dt);
	void	updatePosition(float p_dt);
	int		getType();
};

