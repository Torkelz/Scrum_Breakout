#include "Observer.h"
#include "Scene.h"

Observer::Observer(Scene* p_pScene)
{
	m_pScene = p_pScene;
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
	m_pScene->leftMouseClick( p_mousePosition );
}

void Observer::broadcastRightClick( vec2 p_mousePosition )
{
	m_pScene->rightMouseClick( p_mousePosition );
}

void Observer::broadcastMousePos( vec2 p_mousePosition )
{
	m_pScene->mouseMove( p_mousePosition );
}

void Observer::broadcastKeyPress( USHORT p_key )
{ 
	m_pScene->keyEvent(p_key);
}