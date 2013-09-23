#ifndef BALL_H
#define BALL_H

#include "Object.h"
#include "Sphere.h"

class Ball : public Object
{
private:
	vec3 m_speed;

	std::string		m_texturePath;
public:
	Ball(vec3* p_pPos, vec3* p_pColor, std::string p_objectName);
	~Ball();

	void			update();
	void			updatePosition();
	void			setSpeed(vec3 p_speed);

	std::string*	getTexturePath();
};

#endif BALL_H