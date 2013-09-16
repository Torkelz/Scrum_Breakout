#include "HIDObservable.h"
#include "Observer.h"

HIDObservable::HIDObservable(){}
HIDObservable::~HIDObservable(){}

void HIDObservable::broadcastLeftClick( Vector2 p_mousePosition )
{
	for ( UINT i = 0; i < m_subscribers.size(); i++ )
	{
		m_subscribers.at(i)->broadcastLeftClick( p_mousePosition );
	}
}
void HIDObservable::broadcastRightClick( Vector2 p_mousePosition )
{
	for ( UINT i = 0; i < m_subscribers.size(); i++ )
	{
		m_subscribers.at(i)->broadcastRightClick( p_mousePosition );
	}
}

void HIDObservable::broadcastMousePos( Vector2 p_mousePosition )
{
	for ( UINT i = 0; i < m_subscribers.size(); i++ )
	{
		m_subscribers.at(i)->broadcastMousePos( p_mousePosition );
	}
}

void HIDObservable::broadcastKeyPress( USHORT p_key )
{
	for ( UINT i = 0; i < m_subscribers.size(); i++ )
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
	for ( UINT i = 0; i < m_subscribers.size(); i++ )
	{
		if (p_pObserver->compair( m_subscribers.at(i) ))
		{
			m_subscribers.erase( m_subscribers.begin() + i );
			break;
		}
	}
	m_subscribers.shrink_to_fit();
}