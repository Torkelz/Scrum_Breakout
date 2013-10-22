#include "Header/PUObservable.h"
#include "Header/PUObserver.h"

PUObservable::PUObservable(){}

PUObservable::~PUObservable(){}

void PUObservable::addSubscriber(PUObserver* p_pObserver)
{
	m_pObserver = p_pObserver;
}

void PUObservable::broadcastDeath(int i)
{
	m_pObserver->removePowerUp(i);
}

void PUObservable::broadcastRebirth(PowerUp* p_pPowerUp)
{
	m_pObserver->addPowerUp(p_pPowerUp);
}
