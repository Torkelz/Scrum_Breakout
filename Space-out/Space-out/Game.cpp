#include "Game.h"
#include "Observer.h"
#include "Pad.h"
#include "Block.h"

Game::Game(){}
Game::~Game(){}

void Game::init()
{
	m_pObserver = new Observer(this);
	m_pPad		= new Pad(&Vector3(0.0f, 0.0f, 0.0f), &Vector3(0.56f, 0.56f, 0.56f), "Pad");
	m_loadLevel = LevelGenerator();
	m_loadLevel.loadFile("Levels/level2.txt");
	m_pBlocks = m_loadLevel.getBlocks();
}

void Game::update()
{}

void Game::keyEvent(unsigned short key)
{
	float Rotation = 0;
	if(key == 0x41) // A
	{
		PostQuitMessage(0);
	}
	if(key == 0x44) // D
	{

	}
	if(key == 0x57) // W
	{
		
	}
	if(key == 0x53) // S
	{

	}
	if(key == 0x1B) //ESC
		PostQuitMessage(0);
	

	//if(key == 0x52) // R
	//{
	//}
	//else if( key == 0x46) // F
	//{
	//}
	//else{}

	//DEBUG REMOVE
	if(key == 0x4C) // L
	{
	}
}

void Game::leftMouseClick( Vector2 p_mousePosition ){}

void Game::rightMouseClick( Vector2 p_mousePosition ){}

void Game::mouseMove( Vector2 p_mousePosition )
{
	((Pad*)m_pPad)->setPos( p_mousePosition );
}

Observer* Game::getObserver()
{
	return m_pObserver;
}

Object* Game::getPad()
{
	return m_pPad;
}

BlockLists* Game::getBlocks()
{
	return m_pBlocks;
}