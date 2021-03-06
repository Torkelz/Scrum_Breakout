#ifndef OBSERVER_H
#define OBSERVER_H

#include <Windows.h>
#include <glm.hpp>

using namespace glm;

class Scene;

class Observer
{
public:
	Observer(Scene* p_pScene);
	
	void		broadcastLeftClick( vec2 p_mousePosition );
	void		broadcastRightClick( vec2 p_mousePosition );
	void		broadcastMousePos( vec2 p_mousePosition );
	void		broadcastKeyPress( USHORT p_key );

	int			getIndex();
	bool		compair( Observer* p_pObserver );
private:
	int			m_index;
protected:
	Scene*		m_pScene;
};

#endif