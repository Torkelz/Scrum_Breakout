#include "Game.h"
#include "Observer.h"

Game::Game(){}
Game::~Game(){}

void Game::init()
{
	m_pObserver = new Observer(this);
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

void Game::leftMouseClick( Vector2 p_mousePosition )
{ 
}

void Game::rightMouseClick( Vector2 p_mousePosition )
{
}

void Game::mouseMove( Vector2 p_mousePosition )
{
}

Observer* Game::getObserver()
{
	return m_pObserver;
}