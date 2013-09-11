#include "Observer.h"
#include "Game.h"

Observer::Observer(Game* p_pGame)
{
	m_pGame = p_pGame;
}

int Observer::getIndex()
{
	return m_index;
}

bool Observer::compair( Observer* p_observer )
{
	return m_index == p_observer->getIndex(); 
}

void Observer::broadcastLeftClick( POINT p_mousePosition )
{
	Vector2 t_point;
	t_point.x = p_mousePosition.x;
	t_point.x = p_mousePosition.x;

	m_pGame->leftMouseClick( t_point );
}

void Observer::broadcastRightClick( POINT p_mousePosition )
{
	Vector2 t_point;
	t_point.x = p_mousePosition.x;
	t_point.x = p_mousePosition.x;
	m_pGame->rightMouseClick( t_point );
}

void Observer::broadcastMousePos( POINT p_mousePosition )
{
	Vector2 t_point;
	t_point.x = p_mousePosition.x;
	t_point.x = p_mousePosition.x;
	m_pGame->mouseMove( t_point );
}

void Observer::broadcastKeyPress( USHORT p_key )
{ 
	m_pGame->keyEvent(p_key);
}