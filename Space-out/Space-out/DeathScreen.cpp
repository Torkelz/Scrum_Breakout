#include "DeathScreen.h"
#include "Menu.h"
#include "Observer.h"
#include "D3DTextDevice.h"
#include "Game.h"
#include "HighScore.h"
#include "Menu.h"

DeathScreen::DeathScreen() : Scene()
{}

DeathScreen::~DeathScreen(void){}

void DeathScreen::init(ID3D11Device* p_pDevice, ID3D11DeviceContext* p_pDeviceContext, HWND p_hwnd, int screenWidth, int screenHeight)
{
	m_active = true;
	m_pDevice = p_pDevice;
	m_pDeviceContext = p_pDeviceContext;
	m_pObserver = new Observer(this);
	m_pTextDevice = new D3DTextDevice();

	m_pTextDevice->Initialize(m_pDevice, m_pDeviceContext, p_hwnd, screenWidth, screenHeight);
	
	m_pTextDevice->addSentence("YOU HAVE FAILED YOUR MISSION AND THE EARTH HAS PERISHED", 0, m_pDevice, m_pDeviceContext);
	m_pTextDevice->addSentence("BUT AT LEAST YOU MANAGED TO SCORE: ", 1, m_pDevice, m_pDeviceContext);
	
	m_pTextDevice->addSentence("PRESS SPACE AND TRY NOT TO LOSE THIS TIME!", 2, m_pDevice, m_pDeviceContext);
	m_pTextDevice->addSentence("PRESS BACKSPACE TO RETURN TO MAIN MENU", 3, m_pDevice, m_pDeviceContext);
	m_pTextDevice->addSentence("PRESS H TO SEE IF YOU SCORED MORE THAN ANDRE", 4, m_pDevice, m_pDeviceContext);
	update();
}

void DeathScreen::update()
{
	m_pTextDevice->updateSentenceAt(0, "YOU HAVE FAILED YOUR MISSION AND THE EARTH HAS PERISHED", 250, 50, 1.0f, 1.0f, 1.0f, m_pDeviceContext);
	std::string message = "BUT AT LEAST YOU MANAGED TO SCORE: ";// + IntToString(m_pGame->getScore());
	m_pTextDevice->updateSentenceAt(1, &message[0], 250, 50 + 20, 1.0f, 1.0f, 1.0f, m_pDeviceContext);
	m_pTextDevice->updateSentenceAt(2, "PRESS SPACE AND TRY NOT TO LOSE THIS TIME!", 250, 50 + 70, 1.0f, 1.0f, 1.0f, m_pDeviceContext);
	m_pTextDevice->updateSentenceAt(3, "PRESS BACKSPACE TO RETURN TO MAIN MENU", 250, 50 + 90, 1.0f, 1.0f, 1.0f, m_pDeviceContext);
	m_pTextDevice->updateSentenceAt(4, "PRESS H TO SEE IF YOU SCORED MORE THAN ANDRE", 250, 50 + 110, 1.0f, 1.0f, 1.0f, m_pDeviceContext);
}

void DeathScreen::draw(XMMATRIX* p_pWorld, XMMATRIX* p_pProjection, ID3D11SamplerState* p_sampler, ID3D11RasterizerState* p_raster)
{
	m_pTextDevice->Render(m_pDeviceContext, p_pWorld, p_pProjection, p_sampler, p_raster);
}

void DeathScreen::keyEvent(unsigned short key)
{	
	if(m_active)
	{
		if(key == 0x41) // A
		{
			m_pHighScore->setActive(true);
			m_active = false;
		}
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
		if(key == 0x1B) //ESC
			PostQuitMessage(0);

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

		if(key == 0x20) // SPACE
		{
			m_pGame->setActive(true);
			m_active = false;
		}
	}
}

void DeathScreen::leftMouseClick( vec2 p_mousePosition ){}

void DeathScreen::rightMouseClick( vec2 p_mousePosition ){}

void DeathScreen::mouseMove( vec2 p_mousePosition )
{
}

Observer* DeathScreen::getObserver()
{
	return m_pObserver;
}

void DeathScreen::setHighScore(HighScore* p_pHighScore)
{
	m_pHighScore = p_pHighScore;
}

void DeathScreen::setGame(Game* p_pGame)
{
	m_pGame = p_pGame;
}
