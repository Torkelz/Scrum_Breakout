#pragma once

#include "PowerUp.h"

class PUObserver;

class PUObservable
{
private:
	PUObserver*		m_pObserver;
public:
	PUObservable();
	~PUObservable();
	
	void addSubscriber(PUObserver* p_pObserver);
	void broadcastDeath(int i);
	void broadcastRebirth(PowerUp* p_pPowerUp);
};