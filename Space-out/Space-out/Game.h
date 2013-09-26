#ifndef GAME_H
#define GAME_H

#include <glm.hpp>
#include <gtc\matrix_transform.hpp>
using namespace glm;

class Observer;
class Object;

class Game
{
public:
	Game();
	~Game();

	void		init();

	void		update(float p_screenWidth, float p_dt);
	void		leftMouseClick( vec2 p_mousePosition );
	void		rightMouseClick( vec2 p_mousePosition );
	void		mouseMove( vec2 p_mousePosition );
	void		keyEvent( unsigned short p_key );
	Observer*	getObserver();
	Object*		getPad();
	Object*		getBall();
private:
	Observer*	m_pObserver;
	Object*		m_pPad;
	Object*		m_pBall;
};

#endif	GAME_H