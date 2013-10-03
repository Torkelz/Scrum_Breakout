#ifndef PAD_H
#define PAD_H

#include <string.h>
#include "Object.h"
#include "AABB.h"

class Pad : public Object
{
private:
	vec2	m_mousePos;
	float	m_mouseOffset;
	vec3	m_realPosition;
public:
	Pad(vec3* p_pPos, vec3* p_pColor, std::string p_objectName);
	~Pad();

	void update(mat4 p_translate);
	bool collide(BoundingVolume* p_pVolume);

	void setPos(vec2 p_pos);

	vec2 getMousePos();

	void setMouseOffset(float p_x);
	void changeXCoordXAmount(float p_x);

	vec3 getRealPosition();
};

#endif