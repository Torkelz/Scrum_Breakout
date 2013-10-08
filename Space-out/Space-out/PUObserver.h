#pragma once

#include "PowerUp.h"
class Direct3D;

class PUObserver
{
private:
	Direct3D*	m_pMain;
public:
	PUObserver(Direct3D* p_pMain);
	~PUObserver();

	void		addPowerUp(PowerUp* p_pPowerUp);
	void		removePowerUp(int i);
};

