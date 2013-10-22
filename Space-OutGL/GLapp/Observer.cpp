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

void Observer::broadcastKeyPress( unsigned short p_key )
{ 
	m_pGame->keyEvent(p_key);
}
