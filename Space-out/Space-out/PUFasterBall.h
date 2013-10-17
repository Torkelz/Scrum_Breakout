#pragma once

#include "PowerUp.h"

class PUFasterBall : public PowerUp
{
public:
	PUFasterBall(vec3* p_pos, vec3* p_color, std::string p_objectName);
	~PUFasterBall();
};

