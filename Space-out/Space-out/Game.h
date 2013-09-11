#ifndef GAME_H
#define GAME_H

#include "Vector2.h"

class Observer;

class Game
{
public:
	Game();
	~Game();

	void init();

	void update();
	void leftMouseClick( Vector2 p_mousePosition );
	void rightMouseClick( Vector2 p_mousePosition );
	void mouseMove( Vector2 p_mousePosition );
	void keyEvent( unsigned short p_key );
	Observer* getObserver();
private:
	Observer*	m_pObserver;
};

#endif	GAME_H