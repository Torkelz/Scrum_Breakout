#ifndef BALL_H
#define BALL_H

#include "Object.h"
#include "Sphere.h"

class Ball : public Object
{
private:
	vec3 m_speed;
	vec3 m_realPosition;
	vec2 m_internPosition;
	bool m_stuck;
public:
	Ball(vec3* p_pPos, vec3* p_pColor, std::string p_objectName); // when setting startposition only use x, y not z
	~Ball();

	void			init(vec3 p_origPos, vec3 p_right, vec3 p_down);
	void			update(float p_dt);
	void			updatePosition(float p_dt);
	void			updateBoundingVolume(vec3 p_origPos, vec3 p_right, vec3 p_down);
	void			setPosition(vec3 p_pos);
	void			setSpeed(vec3 p_speed);
	vec3			getSpeed();
	void			speedUp();
	void			speedDown();
	void			setStuck(bool p_stuck);
	bool			getStuck();
	vec3			getRealPosition();
};

#endif BALL_H