#pragma once

#include "PowerUp.h"

class PUOneDown : public PowerUp
{
public:
	PUOneDown(vec3* p_pPos, vec3* p_pColor, std::string p_objectName);
	~PUOneDown();
};

