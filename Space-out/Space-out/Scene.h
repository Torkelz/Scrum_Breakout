#pragma once

#include <glm.hpp>

class Observer;

using namespace glm;

class Scene
{
public:
	Scene(void);
	~Scene(void);
	virtual void					leftMouseClick( vec2 p_mousePosition );
	virtual void					rightMouseClick( vec2 p_mousePosition );
	virtual void					mouseMove( vec2 p_mousePosition );
	virtual void					keyEvent( unsigned short p_key );
	virtual Observer*				getObserver();
	bool							active();
	void							setActive(bool);
protected:
	Observer*						m_pObserver;
	bool							m_active;
};

