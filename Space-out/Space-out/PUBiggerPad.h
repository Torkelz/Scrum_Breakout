#pragma once

#include "PowerUp.h"

class PUBiggerPad : public PowerUp
{
public:
	PUBiggerPad(vec3* p_pPos, vec3* p_pColor, std::string p_objectName);
	~PUBiggerPad();
};

