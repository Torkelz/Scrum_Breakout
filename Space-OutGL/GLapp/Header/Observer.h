#ifndef OBSERVER_H
#define OBSERVER_H

//#include <Windows.h>
#include <glm/glm.hpp>

using namespace glm;

class Game;

class Observer
{
public:
	Observer(Game* p_pGame);
	
	void		broadcastLeftClick( vec2 p_mousePosition );
	void		broadcastRightClick( vec2 p_mousePosition );
	void		broadcastMousePos( vec2 p_mousePosition );
	void		broadcastKeyPress( unsigned short p_key );

	int			getIndex();
	bool		compair( Observer* p_pObserver );
private:
	int			m_index;
protected:
	Game*		m_pGame;
};

#endif