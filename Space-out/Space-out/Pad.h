#ifndef PAD_H
#define PAD_H

#include "Vector2.h"
#include <string.h>
#include "Object.h"

class Pad : public Object
{
private:
public:
	Pad(Vector3* p_pPos, Vector3* p_pColor, std::string p_objectName);
	~Pad();

	void setPos(Vector2 p_pos);
};

#endif