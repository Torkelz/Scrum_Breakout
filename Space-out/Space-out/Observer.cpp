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

bool Observer::compair( Observer* p_pObserver )
{
	return m_index == p_pObserver->getIndex(); 
}

void Observer::broadcastLeftClick( Vector2 p_mousePosition )
{
	m_pGame->leftMouseClick( p_mousePosition );
}

void Observer::broadcastRightClick( Vector2 p_mousePosition )
{
	m_pGame->rightMouseClick( p_mousePosition );
}

void Observer::broadcastMousePos( Vector2 p_mousePosition )
{
	m_pGame->mouseMove( p_mousePosition );
}

void Observer::broadcastKeyPress( USHORT p_key )
{ 
	m_pGame->keyEvent(p_key);
}