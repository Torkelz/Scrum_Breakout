#ifndef PAD_H
#define PAD_H

#include <string.h>
#include "Object.h"
#include "AABB.h"

class Pad : public Object
{
private:
	float m_scale;
	bool m_sticky;
	vec3 m_savedVector;
public:
	Pad(vec3* p_pPos, vec3* p_pColor, std::string p_objectName);
	~Pad();

	void update(mat4 p_translate);
	bool collide(BoundingVolume* p_pVolume);

	void setPos(vec2 p_pos);
	void bigger();
	void smaller();
	float getScale();
	bool getSticky();
	void setSticky(bool p_sticky);
	vec3 getSavedVector();
	void setSavedVector(vec3 p_savedVector);
};

#endif