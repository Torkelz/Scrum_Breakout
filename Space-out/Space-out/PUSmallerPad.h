#pragma once

#include "PowerUp.h"

class PUSmallerPad : public PowerUp
{
public:
	PUSmallerPad(vec3* p_pPos, vec3* p_pColor, std::string p_objectName);
	~PUSmallerPad();
};

