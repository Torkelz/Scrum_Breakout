#include "Scene.h"
#include "Observer.h"

Scene::Scene(void)
{
}

Scene::~Scene(void)
{
}

void Scene::keyEvent(unsigned short key)
{
	//if(key == 0x41) // A
	//{
	//	((Ball*)m_pBall)->setSpeed(vec3(-50.0f, 0.0f, 0.0f) * 3.0f);
	//}
	//if(key == 0x44) // D
	//{
	//	((Ball*)m_pBall)->setSpeed(vec3(50.0f, 0.0f, 0.0f) * 3.0f);
	//}
	//if(key == 0x57) // W
	//{
	//	((Ball*)m_pBall)->setSpeed(vec3(0.0f, -50.0f, 0.0f) * 3.0f);
	//}
	//if(key == 0x53) // S
	//{
	//	((Ball*)m_pBall)->setSpeed(vec3(0.0f, 50.0f, 0.0f) * 3.0f);
	//}
	//if(key == 0x45) // E
	//{
	//	if(!m_pCamera->isCinematic())
	//	{
	//		m_activePlayFieldNext--;
	//		if(m_activePlayFieldNext < 0)
	//			m_activePlayFieldNext = m_nrPlayFields - 1;

	//		m_pCamera->buildPath(	m_playFields[m_activePlayField]->calculateCameraCenterPos(), 
	//								m_playFields[m_activePlayFieldNext]->calculateCameraCenterPos(),
	//								m_originWorld,
	//								4);
	//		m_pCamera->setYaw(m_activePlayFieldNext);
	//		m_pCamera->startCinematic();
	//	}
	//}
	//if(key == 0x51) // Q
	//{
	//	if(!m_pCamera->isCinematic())
	//	{
	//		m_activePlayFieldNext++;
	//		if(m_activePlayFieldNext >= m_nrPlayFields)
	//			m_activePlayFieldNext = 0;

	//		m_pCamera->buildPath(	m_playFields[m_activePlayField]->calculateCameraCenterPos(), 
	//								m_playFields[m_activePlayFieldNext]->calculateCameraCenterPos(),
	//								m_originWorld,
	//								4);
	//		m_pCamera->setYaw(m_activePlayFieldNext);
	//		m_pCamera->startCinematic();
	//	}
	//}
	//if(key == 0x1B) //ESC
	//	PostQuitMessage(0);

	//if(key == 0x52) // R
	//{
	//	PUStickyPad* powerUp = new PUStickyPad(&vec3(0.0f,0.0f,0.0f), &vec3(1.0f,1.0f,1.0f), "PowerUp");
	//	powerUp->setPos(vec3(0.0f, m_pPad->getPos()->y, m_pPad->getPos()->z));
	//	m_pPUObservable->broadcastRebirth(powerUp);
	//	m_powerUps.push_back(powerUp);
	//}
	//if( key == 0x46) // F
	//{
	//	//PUBiggerPad* powerUp = new PUBiggerPad(&vec3(0.0f,0.0f,0.0f), &vec3(1.0f,1.0f,1.0f), "PowerUp");
	//	//powerUp->setPos(vec3(0.0f, m_pPad->getPos()->y, m_pPad->getPos()->z));
	//	//m_pPUObservable->broadcastRebirth(powerUp);
	//	//m_powerUps.push_back(powerUp);
	//}
	//if(key == 0x4C) // L
	//{
	//}
}

void Scene::leftMouseClick( vec2 p_mousePosition )
{
}

void Scene::rightMouseClick( vec2 p_mousePosition ){}

void Scene::mouseMove( vec2 p_mousePosition )
{
}

Observer* Scene::getObserver()
{
	return m_pObserver;
}

bool Scene::active()
{
	return m_active;
}

void Scene::setActive(bool p_active)
{
	m_active = p_active;
}
