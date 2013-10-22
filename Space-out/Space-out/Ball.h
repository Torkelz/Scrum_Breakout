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
	bool m_wallCrash;
	int	 m_wallCounter;
	bool m_padCrash;
	int	 m_padCounter;
	int  m_nrOfSpeedUps;
	int  m_maxSpeedUps;
public:
	Ball(vec3* p_pPos, vec3* p_pColor, std::string p_objectName, vec3 p_speed); // when setting startposition only use x, y not z
	~Ball();

	void			init(vec3 p_origPos, vec3 p_right, vec3 p_down);
	void			update(float p_dt);
	void			updatePosition(float p_dt);
	void			updateBoundingVolume(vec3 p_origPos, vec3 p_right, vec3 p_down);
	void			setSpeed(vec3 p_speed);
	vec3			getSpeed();
	void			speedUp();
	void			speedDown();
	void			setStuck(bool p_stuck);
	bool			getStuck();
	//#### p_origPos = playfield origo ####
	void			setInternalPosition(vec3 p_position, vec3 p_origPos, vec3 p_right, vec3 p_down);
	vec3			getRealPosition();

	void			resetWallCrash();
	bool			getWallCrash();
	void			resetPadCrash();
	bool			getPadCrash();
};

#endif BALL_H