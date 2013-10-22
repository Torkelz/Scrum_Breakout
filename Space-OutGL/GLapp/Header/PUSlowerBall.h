#pragma once

#include "PowerUp.h"

class PUSlowerBall : public PowerUp
{
public:
	PUSlowerBall(vec3* p_pos, vec3* p_color, std::string p_objectName);
	~PUSlowerBall();
};
