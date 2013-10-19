#pragma once
#include "powerup.h"
class PUSplitBall :	public PowerUp
{
public:
	PUSplitBall(vec3* p_pPos, vec3* p_pColor, std::string p_objectName);
	~PUSplitBall();
};

