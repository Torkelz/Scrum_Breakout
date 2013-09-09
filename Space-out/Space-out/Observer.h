#ifndef OBSERVER_H
#define OBSERVER_H

#include <Windows.h>

class Game;

class Observer
{
public:
	Observer(Game* p_pGame);
	
	void		broadcastLeftClick( POINT p_mousePosition );
	void		broadcastRightClick( POINT p_mousePosition );
	void		broadcastMousePos( POINT p_mousePosition );
	void		broadcastKeyPress( USHORT p_key );

	int			getIndex();
	bool		compair( Observer* p_observer );
private:
	int			m_index;
protected:
	Game*		m_pGame;
};

#endif