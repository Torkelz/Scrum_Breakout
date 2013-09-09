#ifndef HIDOBS_H
#define HIDOBS_H

#include <Windows.h>
#include <vector>

class Observer;

class HIDObservable
{
public:
	HIDObservable();
	~HIDObservable();

	void					broadcastLeftClick( POINT p_mousePosition );
	void					broadcastRightClick( POINT p_mousePosition );
	void					broadcastMousePos( POINT p_mousePosition );
	void					broadcastKeyPress( USHORT p_key );
	void					addSubscriber( Observer* p_observer );
	void					removeSubscriber( Observer* p_observer );

private:
	std::vector<Observer*>	m_subscribers;
};

#endif