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
	std::string		m_texturePath;
public:
	Ball(vec3* p_pPos, vec3* p_pColor, std::string p_objectName); // when setting startposition only use x,y not z
	~Ball();

	void			init(vec3 p_origPos, vec3 p_right, vec3 p_down);
	void			update(float p_dt);
	void			updatePosition(float p_dt);
	void			updateBoundingVolume(vec3 p_origPos, vec3 p_right, vec3 p_down);
	void			setPosition(vec3 p_pos);
	void			setSpeed(vec3 p_speed);
	vec3			getSpeed();
	vec3			getRealPosition();


	std::string*	getTexturePath();
};

#endif BALL_H