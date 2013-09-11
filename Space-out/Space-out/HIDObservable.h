#ifndef HIDOBS_H
#define HIDOBS_H

#include <Windows.h>
#include <vector>
#include "Vector2.h"

class Observer;

class HIDObservable
{
public:
	HIDObservable();
	~HIDObservable();

	void					broadcastLeftClick( Vector2 p_mousePosition );
	void					broadcastRightClick( Vector2 p_mousePosition );
	void					broadcastMousePos( Vector2 p_mousePosition );
	void					broadcastKeyPress( USHORT p_key );
	void					addSubscriber( Observer* p_observer );
	void					removeSubscriber( Observer* p_observer );

private:
	std::vector<Observer*>	m_subscribers;
};

#endif