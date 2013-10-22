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
	m_active = false;
	m_pDevice = p_pDevice;
	m_pDeviceContext = p_pDeviceContext;
	m_pObserver = new Observer(this);
	m_pTextDevice = new D3DTextDevice();

	m_pTextDevice->Initialize(m_pDevice, m_pDeviceContext, p_hwnd, screenWidth, screenHeight);
	
	m_pTextDevice->addSentence("Sadly, you have failed your mission and the Earth has perished", 0, m_pDevice, m_pDeviceContext);
	m_pTextDevice->addSentence("Your score was: ", 1, m_pDevice, m_pDeviceContext);
	
	m_pTextDevice->addSentence("Press SPACE to try again", 2, m_pDevice, m_pDeviceContext);
	m_pTextDevice->addSentence("Press BACKSPACE to return to the main menu", 3, m_pDevice, m_pDeviceContext);
	m_pTextDevice->addSentence("Press H to see if you managed to enter, Mr. Nobody", 4, m_pDevice, m_pDeviceContext);
	m_pTextDevice->addSentence("GAME OVER", 5, m_pDevice, m_pDeviceContext);
}

void DeathScreen::update()
{
	m_pTextDevice->updateSentenceAt(0, "Sadly, you have failed your mission and the Earth has perished", 250, 200, 0.29803f, 0.6f, 0.00784f, m_pDeviceContext);
	std::string message = "Your score was: " + IntToString(m_pGame->getScore());
	m_pTextDevice->updateSentenceAt(1, &message[0], 330, 220, 0.29803f, 0.6f, 0.00784f, m_pDeviceContext);
	m_pTextDevice->updateSentenceAt(2, "Press SPACE to try again", 320, 280,  0.29803f, 0.6f, 0.00784f, m_pDeviceContext);
	m_pTextDevice->updateSentenceAt(3, "Press BACKSPACE to return to the main menu", 275, 300,  0.29803f, 0.6f, 0.00784f, m_pDeviceContext);
	m_pTextDevice->updateSentenceAt(4, "Press H to see if you managed to enter the high score list, Mr. Nobody", 235, 320,  0.29803f, 0.6f, 0.00784f, m_pDeviceContext);
	m_pTextDevice->updateSentenceAt(5, "GAME OVER", 350, 100, 1.0f, 0.0f, 0.0f, m_pDeviceContext);
}

void DeathScreen::draw(XMMATRIX* p_pWorld, XMMATRIX* p_pProjection, ID3D11SamplerState* p_sampler, ID3D11RasterizerState* p_raster, ID3D11BlendState* p_Blend)
{
	m_pTextDevice->Render(m_pDeviceContext, p_pWorld, p_pProjection, p_sampler, p_raster, p_Blend);
}

void DeathScreen::keyEvent(unsigned short key)
{	
	if(m_active)
	{
		if(key == 0x1B) //ESC
			PostQuitMessage(0);

		if(key == 0x20) // SPACE
		{
			m_active = false;
		}

		if(key == 0x48) // H
		{
			m_pHighScore->setActive(true);
			m_active = false;
		}

		if(key == 0x08) //BACKSPACE
		{
			m_pMenu->setActive(true);
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

void DeathScreen::setMenu(Menu* p_pMenu)
{
	m_pMenu = p_pMenu;
}