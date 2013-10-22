#include "InstructionScreen.h"
#include "WinScreen.h"
#include "Menu.h"
#include "Observer.h"
#include "D3DTextDevice.h"
#include "Menu.h"

InstructionScreen::InstructionScreen() : Scene()
{}

InstructionScreen::~InstructionScreen(void){}

void InstructionScreen::init(ID3D11Device* p_pDevice, ID3D11DeviceContext* p_pDeviceContext, HWND p_hwnd, int screenWidth, int screenHeight)
{
	m_active = false;
	m_pDevice = p_pDevice;
	m_pDeviceContext = p_pDeviceContext;
	m_pObserver = new Observer(this);
	m_pTextDevice = new D3DTextDevice();

	m_pTextDevice->Initialize(m_pDevice, m_pDeviceContext, p_hwnd, screenWidth, screenHeight);
	
	m_pTextDevice->addSentence("CONTROLS:", 0, m_pDevice, m_pDeviceContext);
	m_pTextDevice->addSentence("Move the mouse to move the Pad.", 1, m_pDevice, m_pDeviceContext);
	m_pTextDevice->addSentence("Left click to release the ball from the sticky pad.", 2, m_pDevice, m_pDeviceContext);
	m_pTextDevice->addSentence("Press SPACE to pause the game.", 3, m_pDevice, m_pDeviceContext);
	m_pTextDevice->addSentence("Press Q or <- to switch sides to the left.", 4, m_pDevice, m_pDeviceContext);
	m_pTextDevice->addSentence("Press E or -> to switch sides to the right.", 5, m_pDevice, m_pDeviceContext);
	
	m_pTextDevice->addSentence("TIPS:", 6, m_pDevice, m_pDeviceContext);
	m_pTextDevice->addSentence("All power ups give points.", 7, m_pDevice, m_pDeviceContext);
	m_pTextDevice->addSentence("You can only switch sides when all balls are lower than the lowest block.", 8, m_pDevice, m_pDeviceContext);
	m_pTextDevice->addSentence("Press BACKSPACE to return to main menu.", 9, m_pDevice, m_pDeviceContext);
}

void InstructionScreen::update()
{
	m_pTextDevice->updateSentenceAt(0, "CONTROLS:", 300, 200, 0.29803f, 0.6f, 0.00784f, m_pDeviceContext);
	m_pTextDevice->updateSentenceAt(1, "Move the mouse to move the Pad.", 300, 220, 0.29803f, 0.6f, 0.00784f, m_pDeviceContext);
	m_pTextDevice->updateSentenceAt(2, "Left click to release the ball from the sticky pad.", 300, 240,  0.29803f, 0.6f, 0.00784f, m_pDeviceContext);
	m_pTextDevice->updateSentenceAt(3, "Press SPACE to pause the game.", 300, 260,  0.29803f, 0.6f, 0.00784f, m_pDeviceContext);
	m_pTextDevice->updateSentenceAt(4, "Press Q or <- to switch sides to the left.", 300, 280,  0.29803f, 0.6f, 0.00784f, m_pDeviceContext);
	m_pTextDevice->updateSentenceAt(5, "Press E or -> to switch sides to the right.", 300, 300,  0.29803f, 0.6f, 0.00784f, m_pDeviceContext);
	m_pTextDevice->updateSentenceAt(6, "TIPS:", 300, 340,  0.29803f, 0.6f, 0.00784f, m_pDeviceContext);
	m_pTextDevice->updateSentenceAt(7, "All power ups give points.", 300, 360,  0.29803f, 0.6f, 0.00784f, m_pDeviceContext);
	m_pTextDevice->updateSentenceAt(8, "You can only switch sides when all balls are lower than the lowest block.", 300, 380,  0.29803f, 0.6f, 0.00784f, m_pDeviceContext);
	m_pTextDevice->updateSentenceAt(9, "Press BACKSPACE to return to main menu.", 500, 390,  1.0f, 0.0f, 0.0f, m_pDeviceContext);
}

void InstructionScreen::draw(XMMATRIX* p_pWorld, XMMATRIX* p_pProjection, ID3D11SamplerState* p_sampler, ID3D11RasterizerState* p_raster)
{
	m_pTextDevice->Render(m_pDeviceContext, p_pWorld, p_pProjection, p_sampler, p_raster);
}

void InstructionScreen::keyEvent(unsigned short key)
{	
	if(m_active)
	{
		if(key == 0x1B) //ESC
			PostQuitMessage(0);

		if(key == 0x08) //BACKSPACE
		{
			m_pMenu->setActive(true);
			m_active = false;
		}
	}
}

void InstructionScreen::leftMouseClick( vec2 p_mousePosition ){}

void InstructionScreen::rightMouseClick( vec2 p_mousePosition ){}

void InstructionScreen::mouseMove( vec2 p_mousePosition )
{
}

Observer* InstructionScreen::getObserver()
{
	return m_pObserver;
}

void InstructionScreen::setMenu(Menu* p_pMenu)
{
	m_pMenu = p_pMenu;
}
