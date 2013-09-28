#include "Game.h"
#include "Observer.h"
#include "Pad.h"
#include "Ball.h"
#include "Block.h"

Game::Game(){}
Game::~Game(){}

void Game::init()
{
	m_pObserver = new Observer(this);
	m_pPad		= new Pad(&vec3(0.0f, 0.0f, 0.0f), &vec3(0.56f, 0.56f, 0.56f), "Pad");
	m_pBall		= new Ball(&vec3(0.0f, 0.0f, 0.0f), &vec3(0.56f, 0.56f, 0.56f), "Ball");
	m_loadLevel = LevelGenerator();
	m_loadLevel.loadFile("Levels/level2.txt");

	m_activePlayField = 0;
	m_originWorld = vec3(0.f,0.f,0.f);


	vec2 size = m_loadLevel.getFieldSize();
	size *= 0.5f;
	vec3 startPosition(m_originWorld.x+size.x, m_originWorld.y + size.y, m_originWorld.z + size.x);
	size *= 2;
	float angle = 0;
	//Create Fields
	for(UINT i = 0; i < m_nrPlayFields; i++, angle += 90)
	{
		m_playFields[i] = new PlayField(startPosition, angle, size);
	}

	for(UINT i = 0; i < m_nrPlayFields; i++)
	{
		m_playFields[i]->init(m_loadLevel.getBlockList(i), m_loadLevel.getNrBlocks());
	}
	m_loadLevel.~LevelGenerator();
}

void Game::update(float p_screenWidth, float p_dt)
{
	vec3 t_pos = *m_pPad->getPos();
	t_pos.x += (p_screenWidth * 0.5f);
	t_pos.x *= 0.125f;
	t_pos.y = -35.0f;
	t_pos.z = 50.0f;

	mat4 padTranslation = translate(mat4(1.0f), t_pos);
	((Pad*)m_pPad)->update(padTranslation);
	((Ball*)m_pBall)->update(p_dt);
	
	if(((Pad*)m_pPad)->collide(m_pBall->getBoundingVolume()))
	{
		vec3 tempSpeed = ((AABB*)m_pPad->getBoundingVolume())->findNewDirection(*m_pBall->getBoundingVolume()->getPosition(), ((Ball*)m_pBall)->getSpeed());
		tempSpeed.y = abs(tempSpeed.y);
		((Ball*)m_pBall)->setSpeed( tempSpeed );
	}
	for(int i = 0; i < m_playFields[m_activePlayField]->getListSize();i++)
	{
		AABB* bv = (AABB*)(m_playFields[m_activePlayField]->getBlock(i)->getBoundingVolume());

		if(  bv->collide(m_pBall->getBoundingVolume()))
		{
			m_playFields[m_activePlayField]->deleteBlock(i);

			vec3 tempSpeed = bv->findNewDirection(*m_pBall->getBoundingVolume()->getPosition(), ((Ball*)m_pBall)->getSpeed());
			((Ball*)m_pBall)->setSpeed( tempSpeed );
		}
	}
}

void Game::keyEvent(unsigned short key)
{
	float Rotation = 0;
	if(key == 0x41) // A
	{
		((Ball*)m_pBall)->setSpeed(vec3(-10.0f, 0.0f, 0.0f));
	}
	if(key == 0x44) // D
	{
		((Ball*)m_pBall)->setSpeed(vec3(10.0f, 0.0f, 0.0f));
	}
	if(key == 0x57) // W
	{
		((Ball*)m_pBall)->setSpeed(vec3(0.0f, 10.0f, 0.0f));
	}
	if(key == 0x53) // S
	{
		((Ball*)m_pBall)->setSpeed(vec3(0.0f, -10.0f, 0.0f));
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
	((Pad*)m_pPad)->setPos( vec2(-p_mousePosition.x, p_mousePosition.y) );
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

PlayField* Game::getField(int p_id)
{
	return m_playFields[p_id];
}

PlayField* Game::getActiveField()
{
	return m_playFields[m_activePlayField];
}

unsigned int Game::getActiveFieldNr()
{
	return m_activePlayField;
}
