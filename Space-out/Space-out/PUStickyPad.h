#pragma once

#include "PowerUp.h"


class PUStickyPad : public PowerUp
{
public:
	PUStickyPad(vec3* p_pPos, vec3* p_pColor, std::string p_objectName);
	~PUStickyPad();
};

