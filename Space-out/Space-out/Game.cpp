#include "Game.h"
#include "Observer.h"
#include "Pad.h"
#include "Ball.h"

Game::Game(){}
Game::~Game(){}

void Game::init()
{
	m_pObserver = new Observer(this);
	m_pPad		= new Pad(&vec3(0.0f, 0.0f, 0.0f), &vec3(0.56f, 0.56f, 0.56f), "Pad");
	m_pBall		= new Ball(&vec3(0.0f, -20.0f, 0.0f), &vec3(0.56f, 0.56f, 0.56f), "Ball");
}

void Game::update(float p_screenWidth)
{
	vec3 t_pos = *m_pPad->getPos();
	t_pos.x -= (p_screenWidth * 0.5f);

	mat4 padTranslation = translate(mat4(1.0f), t_pos);
	((Pad*)m_pPad)->update(padTranslation);
	((Ball*)m_pBall)->update();
	
	if(((Pad*)m_pPad)->collide(m_pBall->getBoundingVolume()))
	{
		int i = 0;
		((Pad*)m_pPad)->collide(m_pBall->getBoundingVolume());
	}
}

void Game::keyEvent(unsigned short key)
{
	float Rotation = 0;
	if(key == 0x41) // A
	{
		((Ball*)m_pBall)->setSpeed(vec3(-1.0f, 0.0f, 0.0f));
	}
	if(key == 0x44) // D
	{
		((Ball*)m_pBall)->setSpeed(vec3(1.0f, 0.0f, 0.0f));
	}
	if(key == 0x57) // W
	{
		((Ball*)m_pBall)->setSpeed(vec3(0.0f, 1.0f, 0.0f));
	}
	if(key == 0x53) // S
	{
		((Ball*)m_pBall)->setSpeed(vec3(0.0f, -1.0f, 0.0f));
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

void Game::leftMouseClick( vec2 p_mousePosition ){}

void Game::rightMouseClick( vec2 p_mousePosition ){}

void Game::mouseMove( vec2 p_mousePosition )
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

Object* Game::getBall()
{
	return m_pBall;
}