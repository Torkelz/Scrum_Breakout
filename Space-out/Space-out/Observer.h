#ifndef OBSERVER_H
#define OBSERVER_H

#include <Windows.h>
#include "Vector2.h"

class Game;

class Observer
{
public:
	Observer(Game* p_pGame);
	
	void		broadcastLeftClick( Vector2 p_mousePosition );
	void		broadcastRightClick( Vector2 p_mousePosition );
	void		broadcastMousePos( Vector2 p_mousePosition );
	void		broadcastKeyPress( USHORT p_key );

	int			getIndex();
	bool		compair( Observer* p_pObserver );
private:
	int			m_index;
protected:
	Game*		m_pGame;
};

#endif