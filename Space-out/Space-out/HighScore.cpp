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
	m_pTextDevice->addSentence("Press SPACE to start a new game", 11, m_pDevice, m_pDeviceContext);
	m_pTextDevice->addSentence("Press BACKSPACE to return to the main menu", 12, m_pDevice, m_pDeviceContext);
	update();
}

void HighScore::update()
{
	for(int i = 1; i < 11; i++)
	{
		std::string message = m_scores[i - 1].name + ": " + IntToString(m_scores[i - 1].score);
		m_pTextDevice->updateSentenceAt(i, &message[0], 390, 60 + m_spacing * i, 0.29803f, 0.6f, 0.00784f, m_pDeviceContext);
	}
	m_pTextDevice->updateSentenceAt(0, "HIGHSCORES", 360, 50, 0.29803f, 0.6f, 0.00784f, m_pDeviceContext);

	m_pTextDevice->updateSentenceAt(11, "Press SPACE to start a new game", 320, 70 + m_spacing * 12, 0.29803f, 0.6f, 0.00784f, m_pDeviceContext);
	m_pTextDevice->updateSentenceAt(12, "Press BACKSPACE to return to the main menu", 300, 70 + m_spacing * 13, 0.29803f, 0.6f, 0.00784f, m_pDeviceContext);
}

void HighScore::draw(XMMATRIX* p_pWorld, XMMATRIX* p_pProjection, ID3D11SamplerState* p_sampler, ID3D11RasterizerState* p_raster, ID3D11BlendState* p_Blend)
{
	m_pTextDevice->Render(m_pDeviceContext, p_pWorld, p_pProjection, p_sampler, p_raster, p_Blend);
}

void HighScore::keyEvent(unsigned short key)
{	
	if(m_active)
	{
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
	std::ofstream file;
	file.open(p_fileName, ios::trunc);

	if(file)
	{
		for (int i = 0; i < 10; i++)
		{
			file << "s " + IntToString(m_scores[i].score) + "\n";
			file << "n " + m_scores[i].name + "\n";
		}
	}
}