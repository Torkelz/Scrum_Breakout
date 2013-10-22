#include "WinScreen.h"
#include "Menu.h"
#include "Observer.h"
#include "D3DTextDevice.h"
#include "Game.h"
#include "HighScore.h"
#include "Menu.h"

WinScreen::WinScreen() : Scene()
{}

WinScreen::~WinScreen(void){}

void WinScreen::init(ID3D11Device* p_pDevice, ID3D11DeviceContext* p_pDeviceContext, HWND p_hwnd, int screenWidth, int screenHeight)
{
	m_active = false;
	m_pDevice = p_pDevice;
	m_pDeviceContext = p_pDeviceContext;
	m_pObserver = new Observer(this);
	m_pTextDevice = new D3DTextDevice();

	m_pTextDevice->Initialize(m_pDevice, m_pDeviceContext, p_hwnd, screenWidth, screenHeight);
	
	m_pTextDevice->addSentence("Congratulations! You have saved the Earth from the evil invaders!", 0, m_pDevice, m_pDeviceContext);
	m_pTextDevice->addSentence("Your score was: ", 1, m_pDevice, m_pDeviceContext);
	
	m_pTextDevice->addSentence("Press SPACE to try again", 2, m_pDevice, m_pDeviceContext);
	m_pTextDevice->addSentence("Press BACKSPACE to return to the main menu", 3, m_pDevice, m_pDeviceContext);
	m_pTextDevice->addSentence("PRESS H TO WATCH THE HIGHSCORE LIST", 4, m_pDevice, m_pDeviceContext);
}

void WinScreen::update()
{
	m_pTextDevice->updateSentenceAt(0, "Congratulations! You have saved the Earth from the evil invaders!", 245, 200, 0.29803f, 0.6f, 0.00784f, m_pDeviceContext);
	std::string message = "Your score was: " + IntToString(m_pGame->getScore());
	m_pTextDevice->updateSentenceAt(1, &message[0], 330, 220, 0.29803f, 0.6f, 0.00784f, m_pDeviceContext);
	m_pTextDevice->updateSentenceAt(2, "Press SPACE to try again", 320, 280,  0.29803f, 0.6f, 0.00784f, m_pDeviceContext);
	m_pTextDevice->updateSentenceAt(3, "Press BACKSPACE to return to the main menu", 275, 300,  0.29803f, 0.6f, 0.00784f, m_pDeviceContext);
	m_pTextDevice->updateSentenceAt(4, "Press H to see if you managed to enter the high score list, Mr. Nobody", 235, 320,  0.29803f, 0.6f, 0.00784f, m_pDeviceContext);
}

void WinScreen::draw(XMMATRIX* p_pWorld, XMMATRIX* p_pProjection, ID3D11SamplerState* p_sampler, ID3D11RasterizerState* p_raster)
{
	m_pTextDevice->Render(m_pDeviceContext, p_pWorld, p_pProjection, p_sampler, p_raster);
}

void WinScreen::keyEvent(unsigned short key)
{	
	if(m_active)
	{
		if(key == 0x41) // A
		{
			m_pHighScore->setActive(true);
			m_active = false;
		}

		if(key == 0x1B) //ESC
			PostQuitMessage(0);

		if(key == 0x20) // SPACE
		{
			m_active = false;
		}

		if(key == 0x08) //BACKSPACE
		{
			m_pMenu->setActive(true);
			m_active = false;
		}
	}
}

void WinScreen::leftMouseClick( vec2 p_mousePosition ){}

void WinScreen::rightMouseClick( vec2 p_mousePosition ){}

void WinScreen::mouseMove( vec2 p_mousePosition )
{
}

Observer* WinScreen::getObserver()
{
	return m_pObserver;
}

void WinScreen::setHighScore(HighScore* p_pHighScore)
{
	m_pHighScore = p_pHighScore;
}

void WinScreen::setGame(Game* p_pGame)
{
	m_pGame = p_pGame;
}

void WinScreen::setMenu(Menu* p_pMenu)
{
	m_pMenu = p_pMenu;
}