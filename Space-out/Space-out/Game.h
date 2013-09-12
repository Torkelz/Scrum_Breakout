#ifndef GAME_H
#define GAME_H

#include "Vector2.h"

class Observer;
class Object;
class Pad;

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
	Object* getPad();

private:
	Observer*	m_pObserver;
	Object*		m_pPad;
};

#endif	GAME_H