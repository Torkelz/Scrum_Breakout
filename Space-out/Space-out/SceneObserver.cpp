#include "SceneObserver.h"
#include "Scene.h"

SceneObserver::SceneObserver(Scene* p_pScene)
{
	m_pScene = p_pScene;
}

int SceneObserver::getIndex()
{
	return m_index;
}

bool SceneObserver::compair( SceneObserver* p_pObserver )
{
	return m_index == p_pObserver->getIndex(); 
}

void SceneObserver::broadcastLeftClick( vec2 p_mousePosition )
{
	m_pScene->leftMouseClick( p_mousePosition );
}

void SceneObserver::broadcastRightClick( vec2 p_mousePosition )
{
	m_pScene->rightMouseClick( p_mousePosition );
}

void SceneObserver::broadcastMousePos( vec2 p_mousePosition )
{
	m_pScene->mouseMove( p_mousePosition );
}

void SceneObserver::broadcastKeyPress( USHORT p_key )
{ 
	m_pScene->keyEvent(p_key);
}
