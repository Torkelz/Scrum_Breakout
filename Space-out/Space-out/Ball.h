#ifndef BALL_H
#define BALL_H

#include "Object.h"

class Ball : public Object
{
private:
	std::string		m_texturePath;
public:
	Ball(Vector3* p_pPos, Vector3* p_pColor, std::string p_objectName);
	~Ball();

	std::string*	getTexturePath();
};

#endif BALL_H