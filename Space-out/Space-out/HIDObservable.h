#ifndef HIDOBS_H
#define HIDOBS_H

#include <Windows.h>
#include <vector>
#include <glm.hpp>

using namespace glm;

class Observer;

class HIDObservable
{
public:
	HIDObservable();
	~HIDObservable();

	void					broadcastLeftClick( vec2 p_mousePosition );
	void					broadcastRightClick( vec2 p_mousePosition );
	void					broadcastMousePos( vec2 p_mousePosition );
	void					broadcastKeyPress( USHORT p_key );
	void					addSubscriber( Observer* p_pObserver );
	void					removeSubscriber( Observer* p_pObserver );

private:
	std::vector<Observer*>	m_subscribers;
};

#endif