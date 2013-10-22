#include "Menu.h"
#include "Observer.h"
#include "D3DTextDevice.h"
#include "Game.h"
#include "HighScore.h"
#include "InstructionScreen.h"

Menu::Menu() : Scene()
{}

Menu::~Menu(void){}

void Menu::init(ID3D11Device* p_pDevice, ID3D11DeviceContext* p_pDeviceContext, HWND p_hwnd, int screenWidth, int screenHeight)
{
	m_active = true;
	m_pDevice = p_pDevice;
	m_pDeviceContext = p_pDeviceContext;
	m_pObserver = new Observer(this);
	m_pTextDevice = new D3DTextDevice();

	m_pTextDevice->Initialize(m_pDevice, m_pDeviceContext, p_hwnd, screenWidth, screenHeight);
	
	m_pTextDevice->addSentence("Press SPACE to start a new game", 0, m_pDevice, m_pDeviceContext);
	m_pTextDevice->addSentence("Press H to look at the high score list", 1, m_pDevice, m_pDeviceContext);
	m_pTextDevice->addSentence("Press I for control information", 2, m_pDevice, m_pDeviceContext);
	//m_pTextDevice->addSentence("Press C to see credits", 3, m_pDevice, m_pDeviceContext);
	update();
}

void Menu::update()
{
	m_pTextDevice->updateSentenceAt(0, "Press SPACE to start a new game", 302, 240, 0.29803f, 0.6f, 0.00784f, m_pDeviceContext);
	m_pTextDevice->updateSentenceAt(1, "Press H to look at the high score list", 300, 270, 0.29803f, 0.6f, 0.00784f, m_pDeviceContext);
	m_pTextDevice->updateSentenceAt(2, "Press I for control information", 315, 300, 0.29803f, 0.6f, 0.00784f, m_pDeviceContext);
	//m_pTextDevice->updateSentenceAt(3, "Press C to see credits", 330, 330, 0.29803f, 0.6f, 0.00784f, m_pDeviceContext);
}

void Menu::draw(XMMATRIX* p_pWorld, XMMATRIX* p_pProjection, ID3D11SamplerState* p_sampler, ID3D11RasterizerState* p_raster, ID3D11BlendState* p_Blend)
{
	m_pTextDevice->Render(m_pDeviceContext, p_pWorld, p_pProjection, p_sampler, p_raster, p_Blend);
}

void Menu::keyEvent(unsigned short key)
{	
	if(m_active)
	{
		if(key == 0x1B) //ESC
			PostQuitMessage(0);

		if(key == 0x20) // SPACE
		{
			m_pGame->setActive(true);
			m_active = false;
		}

		if(key == 0x48) // H
		{
			m_pHighScore->setActive(true);
			m_active = false;
		}

		if(key == 0x49) // I
		{
			m_pInstruction->setActive(true);
			m_active = false;
		}
	}
}

void Menu::leftMouseClick( vec2 p_mousePosition ){}

void Menu::rightMouseClick( vec2 p_mousePosition ){}

void Menu::mouseMove( vec2 p_mousePosition )
{
}

Observer* Menu::getObserver()
{
	return m_pObserver;
}

void Menu::setHighScore(HighScore* p_pHighScore)
{
	m_pHighScore = p_pHighScore;
}

void Menu::setGame(Game* p_pGame)
{
	m_pGame = p_pGame;
}

void Menu::setInstruction(InstructionScreen* p_pInstr)
{
	m_pInstruction = p_pInstr;
}