#include "Header/Observer.h"
#include "Header/Game.h"

Observer::Observer(Game* p_pGame)
{
	m_pGame = p_pGame;
	m_index = 0;
}

int Observer::getIndex()
{
	return m_index;
}

bool Observer::compair( Observer* p_pObserver )
{
	return m_index == p_pObserver->getIndex(); 
}

void Observer::broadcastLeftClick( vec2 p_mousePosition )
{
	m_pGame->leftMouseClick( p_mousePosition );
}

void Observer::broadcastRightClick( vec2 p_mousePosition )
{
	m_pGame->rightMouseClick( p_mousePosition );
}

void Observer::broadcastMousePos( vec2 p_mousePosition )
{
	m_pGame->mouseMove( p_mousePosition );
}

void Observer::broadcastKeyPress( unsigned short p_key )
{ 
	m_pGame->keyEvent(p_key);
}
