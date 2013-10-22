#include "Header/PUObserver.h"
#include "Header/OpenGL.h"

PUObserver::PUObserver(OpenGL* p_pMain)
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
