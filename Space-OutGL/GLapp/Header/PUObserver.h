#pragma once

#include "PowerUp.h"
class OpenGL;

class PUObserver
{
private:
	OpenGL*	m_pMain;
public:
	PUObserver(OpenGL* p_pMain);
	~PUObserver();

	void		addPowerUp(PowerUp* p_pPowerUp);
	void		removePowerUp(int i);
};

