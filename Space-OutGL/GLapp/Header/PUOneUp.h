#pragma once

#include "PowerUp.h"

class PUOneUp : public PowerUp
{
public:
	PUOneUp(vec3* p_pPos, vec3* p_pColor, std::string p_objectName);
	~PUOneUp();
};

