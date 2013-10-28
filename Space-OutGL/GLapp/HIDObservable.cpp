#include "Header/HIDObservable.h"
#include "Header/Observer.h"
#include <vector>

HIDObservable::HIDObservable(){}
HIDObservable::~HIDObservable(){}

void HIDObservable::broadcastKeyPress( unsigned short p_key )
{
	for ( unsigned int i = 0; i < m_subscribers.size(); i++ )
	{
		m_subscribers.at(i)->broadcastKeyPress( p_key );
	}
}

void HIDObservable::addSubscriber( Observer* p_pObserver )
{
	m_subscribers.push_back( p_pObserver );
}

void HIDObservable::removeSubscriber( Observer* p_pObserver )
{
	for ( unsigned int i = 0; i < m_subscribers.size(); i++ )
	{
		if (p_pObserver->compair( m_subscribers.at(i) ))
		{
			m_subscribers.erase( m_subscribers.begin() + i );
			break;
		}
	}
}