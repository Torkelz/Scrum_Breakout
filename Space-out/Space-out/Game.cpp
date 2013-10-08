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
	m_pPad		= new Pad(&vec3(0.0f, 125.0f, 0.0f), &vec3(0.56f, 0.56f, 0.56f), "Pad");
	m_pBall		= new Ball(&vec3(50.0f, 100.0f, 0.0f), &vec3(0.56f, 0.56f, 0.56f), "Ball");
	m_loadLevel = LevelGenerator();
	m_loadLevel.loadFile("Levels/level2.txt");

	m_activePlayField = 3;
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
		int t = i % 2;
		m_playFields[i]->init(m_loadLevel.getBlockList(i), m_loadLevel.getNrBlocks(), t);
	}

	//Set ball bounding box
	PlayField* pf = m_playFields[m_activePlayField];
	((Ball*)m_pBall)->init(pf->getOriginalPosition(), pf->getRightDir(), pf->getDownDir());

	m_loadLevel.~LevelGenerator();
}

void Game::update(float p_screenWidth, float p_dt)
{
	vec3 padPos = *m_pPad->getPos();
	/*vec3 t_pos = *m_pPad->getPos();
	t_pos.x += (p_screenWidth * 0.5f);
	t_pos.x *= 0.125f;
	t_pos.y = -35.0f;
	t_pos.z = 50.0f;*/
	PlayField* pf = m_playFields[m_activePlayField];
	vec3 t_pos = pf->getOriginalPosition();
	t_pos -= pf->getRightDir() * padPos.x;
	t_pos += pf->getDownDir() * padPos.y;
	

	mat4 padTranslation = translate(mat4(1.0f), t_pos);

	
	((Pad*)m_pPad)->update(padTranslation, pf->getRotationMatrix());
	((Ball*)m_pBall)->update(p_dt);
	((Ball*)m_pBall)->updateBoundingVolume(pf->getOriginalPosition(),pf->getRightDir(),pf->getDownDir());
	pf = NULL;

	//Pad vs Ball
	if(((Pad*)m_pPad)->collide(m_pBall->getBoundingVolume()))
	{
		vec3 s = ((Ball*)m_pBall)->getSpeed();
		vec3 tempSpeed = ((AABB*)m_pPad->getBoundingVolume())->findNewDirection(*m_pBall->getBoundingVolume()->getPosition(), s);
		tempSpeed.y = -abs(tempSpeed.y);
		((Ball*)m_pBall)->setSpeed( tempSpeed );
	}
	//Ball vs boxes
	for(int i = 0; i < m_playFields[m_activePlayField]->getListSize();i++)
	{
		AABB* bv = (AABB*)(m_playFields[m_activePlayField]->getBlock(i)->getBoundingVolume());

		if(bv->collide(m_pBall->getBoundingVolume()))
		{
			vec3 s = ((Ball*)m_pBall)->getSpeed();
			vec3 tempSpeed = bv->findNewDirection(*m_pBall->getBoundingVolume()->getPosition(), s);
			//tempSpeed.y = -tempSpeed.y;
			((Ball*)m_pBall)->setSpeed( tempSpeed );

			m_playFields[m_activePlayField]->deleteBlock(i);
			bv = NULL;
			break;
		}
	}
	//Ball vs Borders
	for(unsigned int i = 0; i < m_playFields[m_activePlayField]->getNrBorders(); i++)
	{
		AABB* bv = (AABB*)(m_playFields[m_activePlayField]->getCollisionBorder(i));

		if(  bv->collide(m_pBall->getBoundingVolume()))
		{
			vec3 tempSpeed = bv->findNewDirection(*m_pBall->getBoundingVolume()->getPosition(), ((Ball*)m_pBall)->getSpeed());
			tempSpeed.y = tempSpeed.y;
			((Ball*)m_pBall)->setSpeed( tempSpeed );
			break;
		}
	}
	//Pad vs Borders NEEDS FINE TUNING
	for(unsigned int i = 0; i < m_playFields[m_activePlayField]->getNrBorders()-2; i++)
	{
		AABB* bv = (AABB*)(m_playFields[m_activePlayField]->getCollisionBorder(i));

		if(  bv->collide(m_pPad->getBoundingVolume()))
		{
			vec3 Bcenter = *bv->getPosition();
			vec3 Pcenter = *m_pPad->getBoundingVolume()->getPosition();
			Bcenter.y = Pcenter.y;

			vec3 dir = normalize(Pcenter - Bcenter);

			float x = dot(dir, m_playFields[m_activePlayField]->getRightDir());
			x *= -1.f;
			if(i == 0) //Hit Left side
			{
				((Pad*)(m_pPad))->changeXCoordXAmount(x*2); // NEEDS FINE TUNING
			}
			else //Hit Right side
			{
				((Pad*)(m_pPad))->changeXCoordXAmount(x*2);
			}


			break;
		}
	}
}

void Game::keyEvent(unsigned short key)
{
	float Rotation = 0;
	if(key == 0x41) // A
	{
		((Ball*)m_pBall)->setSpeed(vec3(-50.0f, 0.0f, 0.0f));
	}
	if(key == 0x44) // D
	{
		((Ball*)m_pBall)->setSpeed(vec3(50.0f, 0.0f, 0.0f));
	}
	if(key == 0x57) // W
	{
		((Ball*)m_pBall)->setSpeed(vec3(0.0f, -50.0f, 0.0f));
	}
	if(key == 0x53) // S
	{
		((Ball*)m_pBall)->setSpeed(vec3(0.0f, 50.0f, 0.0f));
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
