#ifndef BALL_H
#define BALL_H

#include "Object.h"
#include "Sphere.h"

class Ball : public Object
{
private:
	vec3			m_speed;
	bool			m_stuck;
public:
	Ball(vec3* p_pPos, vec3* p_pColor, std::string p_objectName);
	~Ball();

	void			update(float p_dt);
	void			updatePosition(float p_dt);
	void			setPosition(vec3 p_pos);
	void			setSpeed(vec3 p_speed);
	vec3			getSpeed();
	void			speedUp();
	void			speedDown();
	void			setStuck(bool p_stuck);
	bool			getStuck();
};

#endif BALL_H