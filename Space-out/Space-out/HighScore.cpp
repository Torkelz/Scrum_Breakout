#include "HighScore.h"
#include "Observer.h"
#include "D3DTextDevice.h"
#include "Game.h"
#include "Menu.h"

HighScore::HighScore() : Scene()
{
	loadHighScoreListFromFile("highscores.txt");
	m_spacing = 30;
}

HighScore::HighScore(Game* p_pGame, Menu* p_pMenu) : Scene()
{
	m_pGame = p_pGame;
	m_pMenu = p_pMenu;
}

HighScore::~HighScore(){}

void HighScore::init(ID3D11Device* p_pDevice, ID3D11DeviceContext* p_pDeviceContext, HWND p_hwnd, int screenWidth, int screenHeight)
{
	m_active = false;
	m_pDevice = p_pDevice;
	m_pDeviceContext = p_pDeviceContext;
	m_pObserver = new Observer(this);
	m_pTextDevice = new D3DTextDevice();

	m_pTextDevice->Initialize(m_pDevice, m_pDeviceContext, p_hwnd, screenWidth, screenHeight);
	
	m_pTextDevice->addSentence("HIGHSCORES", 0, m_pDevice, m_pDeviceContext);
	for(int i = 1; i < 11; i++)
	{
		m_pTextDevice->addSentence("", i, m_pDevice, m_pDeviceContext);
	}
	m_pTextDevice->addSentence("PRESS SPACE TO START NEW GAME", 11, m_pDevice, m_pDeviceContext);
	m_pTextDevice->addSentence("PRESS BACKSPACE TO RETURN TO MAIN MENU", 12, m_pDevice, m_pDeviceContext);
	update();
}

void HighScore::update()
{
	for(int i = 1; i < 11; i++)
	{
		std::string message = m_scores[i - 1].name + ": " + IntToString(m_scores[i - 1].score);
		m_pTextDevice->updateSentenceAt(i, &message[0], 270, 60 + m_spacing * i, 1.0f, 1.0f, 1.0f, m_pDeviceContext);
	}
	m_pTextDevice->updateSentenceAt(0, "HIGHSCORES", 250, 50, 1.0f, 1.0f, 1.0f, m_pDeviceContext);

	m_pTextDevice->updateSentenceAt(11, "PRESS SPACE TO START NEW GAME", 250, 70 + m_spacing * 11, 1.0f, 1.0f, 1.0f, m_pDeviceContext);
	m_pTextDevice->updateSentenceAt(12, "PRESS BACKSPACE TO RETURN TO MAIN MENU", 250, 70 + m_spacing * 12, 1.0f, 1.0f, 1.0f, m_pDeviceContext);
}

void HighScore::draw(XMMATRIX* p_pWorld, XMMATRIX* p_pProjection, ID3D11SamplerState* p_sampler, ID3D11RasterizerState* p_raster)
{
	m_pTextDevice->Render(m_pDeviceContext, p_pWorld, p_pProjection, p_sampler, p_raster);
}

void HighScore::keyEvent(unsigned short key)
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
		if(m_active)
		{
			m_pGame->setActive(true);
			m_active = false;
		}
	}
	if(key == 0x08) //BACKSPACE
	{
		if(m_active)
		{
			m_pMenu->setActive(true);
			m_active = false;
		}
	}
}

void HighScore::leftMouseClick( vec2 p_mousePosition ){}

void HighScore::rightMouseClick( vec2 p_mousePosition ){}

void HighScore::mouseMove( vec2 p_mousePosition ){}

Observer* HighScore::getObserver()
{
	return m_pObserver;
}

void HighScore::setMenu(Menu* p_pMenu)
{
	m_pMenu = p_pMenu;
}

void HighScore::setGame(Game* p_pGame)
{
	m_pGame = p_pGame;
}

void HighScore::loadHighScoreListFromFile(std::string p_fileName)
{
	std::fstream file(p_fileName);

	if(file)
	{
		std::string line;
		std::string prefix;
		int index = 0;

		while(file.eof() == false)
		{
			prefix = "NULL"; //leave nothing from the previous iteration
			std::stringstream lineStream;

			std::getline(file, line);
			lineStream << line;
			lineStream >> prefix;

			if(prefix == "s")
			{
				int entryScore;
				lineStream >> entryScore;
				m_scores[index].score = entryScore;
			}
			else if(prefix == "n")
			{
				std::string entrantName;
				lineStream >> entrantName;
				m_scores[index].name = entrantName;
				index++;
			}
		}
	}
}

bool HighScore::addHighScore(int p_score)
{
	HighScoreEntry newScore;
	newScore.score = p_score;
	newScore.name = "Mr.Nobody";
	bool swapped = false;
	for (int i = 0; i < 10; i++)
	{
		HighScoreEntry temp;
		if(newScore.score > m_scores[i].score)
		{
			temp = m_scores[i];
			m_scores[i] = newScore;
			newScore = temp;
			swapped = true;
		}
	}

	if(swapped)
		writeHighScoreToFile("highscores.txt");

	return swapped;
}

void HighScore::writeHighScoreToFile(std::string p_fileName)
{
	//DO STUFF
}