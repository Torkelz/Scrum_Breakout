#pragma once

#include "PowerUp.h"

class PUExplosiveBall : public PowerUp
{
public:
	PUExplosiveBall(vec3* p_pPos, vec3* p_pColor, std::string p_objectName);
	~PUExplosiveBall();
};

