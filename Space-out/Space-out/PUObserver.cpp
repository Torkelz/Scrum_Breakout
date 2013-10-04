#include "PUObserver.h"
#include "Direct3D.h"

PUObserver::PUObserver(Direct3D* p_pMain)
{
	m_pMain = p_pMain;
}

PUObserver::~PUObserver(){}

void PUObserver::addPowerUp(PowerUp* p_pPowerUp)
{
	m_pMain->addPowerUp(p_pPowerUp);
}

void PUObserver::removePowerUp(int i)
{
	m_pMain->removePowerUp(i);
}