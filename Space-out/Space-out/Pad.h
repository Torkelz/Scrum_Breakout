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
	float	m_scale;
	bool	m_sticky;
	vec3	m_savedVector;
	int		m_nrOfScales;
	int		m_maxScales;
public:
	Pad(vec3* p_pPos, vec3* p_pColor, std::string p_objectName, float p_size);
	~Pad();

	void update(mat4 p_translate, mat4 p_rotation);
	bool collide(BoundingVolume* p_pVolume);

	vec2 getMousePos();

	void setMouseOffset(float p_x);
	void changeXCoordXAmount(float p_x);

	vec3 getRealPosition();
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