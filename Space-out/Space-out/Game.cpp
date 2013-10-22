#include "Game.h"
#include "Observer.h"
#include "Pad.h"
#include "Ball.h"
#include "Block.h"
#include "SceneInclude.h"

Game::Game(){}
Game::~Game(){}

void Game::init(PUObserver* p_pPUObserver, DIFFICULTIES p_diff)
{
	m_active = false;
	m_paused = false;
	Difficulties diff = Difficulties();
	diff.setInitValues(p_diff);
	m_sDiffData = diff.getDifficultyValues();

	m_player = SPlayer((short)m_sDiffData.lives, m_sDiffData.multiplier);

	m_pObserver = new Observer(this);
	m_pPad		= new Pad(&vec3(0.0f, 125.0f, 0.0f), &vec3(0.56f, 0.56f, 0.56f), "Pad", m_sDiffData.padStartSize);
	m_pBall		= new Ball(&vec3(50.0f, 100.0f, 0.0f), &vec3(0.56f, 0.56f, 0.56f), "Ball", m_sDiffData.ballStartSpeed);
	m_loadLevel = LevelGenerator();
	m_loadLevel.loadFile("Levels/level2.txt");

	m_newVolume = 0.5f;

	m_activePlayField = 0;
	m_activePlayFieldNext = m_activePlayField;

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
	resetBall(pf);

	m_pCamera = new Camera();
	m_pCamera->init(pf->calculateCameraCenterPos());
	m_pCamera->setViewMatrix();
	m_pCamera->createProjectionMatrix(PI*0.25f,(float)CLIENTWIDTH/CLIENTHEIGHT, 1.0f, 500.0f);
	//m_pCamera->setYaw(m_activePlayField);

	m_loadLevel.~LevelGenerator();
	m_pPUObservable = new PUObservable();
	m_pPUObservable->addSubscriber(p_pPUObserver);

	loadSounds();
	m_soundManager.play(m_pSoundList.at(BACKGROUND), 0);
	m_soundManager.setVolume(0.0f, 0);
	
	int tempPowerUpSize = m_powerUps.size() - 1;
	for(int i = tempPowerUpSize; i >= 0; i--)
	{
		m_pPUObservable->broadcastDeath(i);
		m_powerUps.erase(m_powerUps.begin() + i);
	}
}

void Game::update(float p_screenWidth, float p_dt)
{
	m_soundManager.update();

	if(!m_paused)
	{
		m_pCamera->updateCameraPos(p_dt);

		if(m_pCamera->timeToChange())
		{
			m_activePlayField = m_activePlayFieldNext;
			int tempPowerUpSize = m_powerUps.size() - 1;
			for(int i = tempPowerUpSize; i >= 0; i--)
			{
				m_pPUObservable->broadcastDeath(i);
				m_powerUps.erase(m_powerUps.begin() + i);
			}
		}

		if(!m_pCamera->isCinematic())
		{
			if(m_counter > 0.0f)
			{
				m_counter -= p_dt;
			}
			else
				((Pad*)m_pPad)->setSticky(false);

			if(m_padCounter > 0)
			{
				m_padCounter--;
			}
			else
				m_padCrash = false;

			if(m_wallCounter > 0)
			{
				m_wallCounter--;
			}
			else
				m_wallCrash = false;

			vec3 padPos = *m_pPad->getPos();
			PlayField* pf = m_playFields[m_activePlayField];
			vec3 t_pos = pf->getOriginalPosition();
			t_pos -= pf->getRightDir() * padPos.x;
			t_pos += pf->getDownDir() * padPos.y;

			mat4 padTranslation = translate(mat4(1.0f), t_pos);
			mat4 padRotation;
			if(m_activePlayField == 0 || m_activePlayField == 2)
				padRotation = m_playFields[0]->getRotationMatrix();
			else
				padRotation = m_playFields[3]->getRotationMatrix();

			((Pad*)m_pPad)->update(padTranslation, padRotation);

			if(!((Ball*)m_pBall)->getStuck())
			{
				((Ball*)m_pBall)->update(p_dt);
				((Ball*)m_pBall)->updateBoundingVolume(pf->getOriginalPosition(),pf->getRightDir(),pf->getDownDir());

				//Pad vs Ball
				if(((Pad*)m_pPad)->collide(m_pBall->getBoundingVolume()) && !m_padCrash)
				{
					if(!((Pad*)m_pPad)->getSticky())
					{
						vec3 tempSpeed = ((AABB*)m_pPad->getBoundingVolume())->findNewDirection(*m_pBall->getBoundingVolume()->getPosition(), ((Ball*)m_pBall)->getSpeed());
					
						tempSpeed.y = -abs(tempSpeed.y);
						((Ball*)m_pBall)->setSpeed( tempSpeed );
						m_padCrash = true;
						m_padCounter = 4;
					}
					else
					{
						((Ball*)m_pBall)->setStuck(true);
						((Pad*)m_pPad)->setSavedVector( ((Ball*)m_pBall)->getRealPosition()- *m_pPad->getBoundingVolume()->getPosition() );
					}
				}

				// ## BLOCKS ##
				for(int i = 0; i < m_playFields[m_activePlayField]->getListSize();i++)
				{
					AABB* bv = (AABB*)(m_playFields[m_activePlayField]->getBlock(i)->getBoundingVolume());

					if(bv->collide(m_pBall->getBoundingVolume()))
					{
						m_soundManager.play(m_pSoundList[COLLISION], 1);
						m_soundManager.setVolume(0.35f, 1);
						vec3 s = ((Ball*)m_pBall)->getSpeed();
						vec3 tempSpeed = bv->findNewDirection(*m_pBall->getBoundingVolume()->getPosition(), s);
						((Ball*)m_pBall)->setSpeed( tempSpeed );
						powerUpSpawn(*m_playFields[m_activePlayField]->getBlock(i)->getPos());
						m_playFields[m_activePlayField]->deleteBlock(i);
						bv = NULL;
						break;
					}
				}
				// ## WALLS ##
				for(unsigned int i = 0; i < m_playFields[m_activePlayField]->getNrBorders(); i++)
				{
					AABB* bv = (AABB*)(m_playFields[m_activePlayField]->getCollisionBorder(i));

					if(  bv->collide(m_pBall->getBoundingVolume()) && !m_wallCrash)
					{
						if (i == 3)
						{
							resetBall(pf);
							m_sDiffData.lives--;
						}
						vec3 tempSpeed = bv->findNewDirection(*m_pBall->getBoundingVolume()->getPosition(), ((Ball*)m_pBall)->getSpeed());
						tempSpeed.y = tempSpeed.y;
						((Ball*)m_pBall)->setSpeed( tempSpeed );
						m_wallCrash = true;
						m_wallCounter = 6;
						break;
					}
				}
			}
			else
			{
				((Ball*)m_pBall)->setInternalPosition( *m_pPad->getBoundingVolume()->getPosition() + ((Pad*)m_pPad)->getSavedVector(),pf->getOriginalPosition(),pf->getRightDir(), pf->getDownDir() );
				((Ball*)m_pBall)->updateBoundingVolume(pf->getOriginalPosition(),pf->getRightDir(),pf->getDownDir());
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

					vec3 dir = Pcenter - Bcenter;
					if(length(dir) > 0.0f)
						dir = normalize(dir);

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

			padPos = ((Pad*)m_pPad)->getRealPosition();
			for(unsigned int i = 0; i < m_powerUps.size(); i++)
			{
				// Update position for power ups.
				mat4 powTranslation;// = translate(mat4(1.0f), vec3(m_powerUps.at(i)->getPos()->x, m_powerUps.at(i)->getPos()->y, t_pos.z));
				if (m_activePlayField == 0 || m_activePlayField == 2)
					powTranslation = translate(mat4(1.0f), vec3(m_powerUps.at(i)->getPos()->x, m_powerUps.at(i)->getPos()->y, padPos.z)); // Translate powerup
				else
					powTranslation = translate(mat4(1.0f), vec3(padPos.x, m_powerUps.at(i)->getPos()->y, m_powerUps.at(i)->getPos()->z));

				m_powerUps.at(i)->update(p_dt, powTranslation);
				AABB* bv = (AABB*)(m_powerUps.at(i)->getBoundingVolume());

				if( bv->collide(m_pPad->getBoundingVolume()) )
				{
					//Power Up catch.
					powerUpCheck(m_powerUps.at(i)->getType());
					m_pPUObservable->broadcastDeath(i);
					m_powerUps.erase(m_powerUps.begin() + i);
				}
				if( bv->collide(m_playFields[m_activePlayField]->getCollisionBorder(3)))
				{
					//Power Up missed.
					m_pPUObservable->broadcastDeath(i);
					m_powerUps.erase(m_powerUps.begin() + i);
				}
			}
			// ## COLLISION STUFF END ##

			if(pf->getListSize() <= 0) // If playfield is empty move the ball to the pad.
			{
				resetBall(pf);
			}

			pf = NULL;

			// TEST LIVES
			if (m_sDiffData.lives <= 0)
			{
				m_active = false;
				m_deathScene->setActive(true);
			}

			int nrOfRemainingBlocks = 0;
			for (int pl = 0; pl < 4; pl++)
			{
				nrOfRemainingBlocks += m_playFields[pl]->getListSize();
			}
			if(nrOfRemainingBlocks <= 0)
			{
				m_active = false;
				m_winScene->setActive(true);
			}
		}
	}
	// SOUND IS OFF HERE REMOVE WHEN NEEDING ZE SOUNDS
	m_soundManager.setPauseAll(true);
}

void Game::keyEvent(unsigned short key)
{
	if(m_active)
	{
		if(key == 0x41) // A
		{
			((Ball*)m_pBall)->setSpeed(vec3(-50.0f, 0.0f, 0.0f) * 3.0f);
		}
		if(key == 0x44) // D
		{
			((Ball*)m_pBall)->setSpeed(vec3(50.0f, 0.0f, 0.0f) * 3.0f);
		}
		if(key == 0x57) // W
		{
			((Ball*)m_pBall)->setSpeed(vec3(0.0f, -50.0f, 0.0f) * 3.0f);
		}
		if(key == 0x53) // S
		{
			((Ball*)m_pBall)->setSpeed(vec3(0.0f, 50.0f, 0.0f) * 3.0f);
		}
		if(key == 0x45) // E
		{
			if(!m_pCamera->isCinematic())
			{
				m_activePlayFieldNext--;
				if(m_activePlayFieldNext < 0)
					m_activePlayFieldNext = m_nrPlayFields - 1;

				m_pCamera->buildPath(	m_playFields[m_activePlayField]->calculateCameraCenterPos(), 
										m_playFields[m_activePlayFieldNext]->calculateCameraCenterPos(),
										m_originWorld,
										4);
				m_pCamera->setYaw(m_activePlayFieldNext);
				m_pCamera->startCinematic();
			}
		}
		if(key == 0x51) // Q
		{
			if(!m_pCamera->isCinematic())
			{
				m_activePlayFieldNext++;
				if(m_activePlayFieldNext >= m_nrPlayFields)
					m_activePlayFieldNext = 0;

				m_pCamera->buildPath(	m_playFields[m_activePlayField]->calculateCameraCenterPos(), 
										m_playFields[m_activePlayFieldNext]->calculateCameraCenterPos(),
										m_originWorld,
										4);
				m_pCamera->setYaw(m_activePlayFieldNext);
				m_pCamera->startCinematic();
			}
		}
		if(key == 0x1B) //ESC
			PostQuitMessage(0);

		if(key == 0x52) // R
		{
			PUStickyPad* powerUp = new PUStickyPad(&vec3(0.0f,0.0f,0.0f), &vec3(1.0f,1.0f,1.0f), "PowerUp");
			powerUp->setPos(vec3(0.0f, m_pPad->getPos()->y, m_pPad->getPos()->z));
			m_pPUObservable->broadcastRebirth(powerUp);
			m_powerUps.push_back(powerUp);
		}
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
			m_paused = (m_paused + 1) % 2;
		}
	}
}

void Game::leftMouseClick( vec2 p_mousePosition )
{
	if(m_active)
	{
		((Ball*)m_pBall)->setStuck(false);
		vec3 tempSpeed = ((AABB*)m_pPad->getBoundingVolume())->findNewDirection(*m_pBall->getBoundingVolume()->getPosition(), ((Ball*)m_pBall)->getSpeed());
		tempSpeed.y = -abs(tempSpeed.y);
		((Ball*)m_pBall)->setSpeed( tempSpeed );
	}
}

void Game::rightMouseClick( vec2 p_mousePosition ){}

void Game::mouseMove( vec2 p_mousePosition )
{
	if(m_active)
	{
		if(p_mousePosition.x >= m_screenBorders.x &&
			p_mousePosition.x <= m_screenBorders.y &&
			p_mousePosition.y >= m_screenBorders.z &&
			p_mousePosition.y <= m_screenBorders.w)
		((Pad*)m_pPad)->setPos( vec2(-p_mousePosition.x, p_mousePosition.y) );
	}
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

Camera*	Game::getCamera()
{
	return m_pCamera;
}

void Game::powerUpCheck(int i)
{
	switch (i)
	{
	case FASTERBALL:
		((Ball*)m_pBall)->speedUp();
		break;
	case SLOWERBALL:
		((Ball*)m_pBall)->speedDown();
		break;
	case BIGGERPAD:
		((Pad*)m_pPad)->bigger();
		break;
	case SMALLERPAD:
		((Pad*)m_pPad)->smaller();
		break;
	case STICKYPAD:
		((Pad*)m_pPad)->setSticky(true);
		m_counter = 10.0f;
	default:
		break;
	}

	//Remember to remove the power up outside this function!
}

void Game::powerUpSpawn(vec3 pos)
{
	if(m_powerUps.size() < 10)
	{
		int chance = 25;
		int r = rand() % 100;
		// chance for powerups
		if(r < chance * m_sDiffData.dropRate)
		{
			r = rand() % 3;
			switch (r)
			{
			case FASTERBALL:
				{
					PUFasterBall* powerUp = new PUFasterBall(&vec3(0.0f,0.0f,0.0f), &vec3(1.0f,1.0f,1.0f), "PowerUp");
					powerUp->setPos(pos);
					((AABB*)powerUp->getBoundingVolume())->calculateAngle(false, false);
					m_pPUObservable->broadcastRebirth(powerUp);
					m_powerUps.push_back(powerUp);
				}
				break;
			case BIGGERPAD:
				{
					PUBiggerPad* powerUp = new PUBiggerPad(&vec3(0.0f,0.0f,0.0f), &vec3(1.0f,1.0f,1.0f), "PowerUp");
					powerUp->setPos(pos);
					((AABB*)powerUp->getBoundingVolume())->calculateAngle(false, false);
					m_pPUObservable->broadcastRebirth(powerUp);
					m_powerUps.push_back(powerUp);
				}
				break;
			case STICKYPAD:
				{
					PUStickyPad* powerUp = new PUStickyPad(&vec3(0.0f,0.0f,0.0f), &vec3(1.0f,1.0f,1.0f), "PowerUp");
					powerUp->setPos(pos);
					((AABB*)powerUp->getBoundingVolume())->calculateAngle(false, false);
					m_pPUObservable->broadcastRebirth(powerUp);
					m_powerUps.push_back(powerUp);
				}
			default:
				break;
			}
		} // Drop chance for powerdowns!
		else if(r < chance)
		{
			r = rand() % 2;
			switch(r)
			{
			case SLOWERBALL:
				{
					PUSlowerBall* powerUp = new PUSlowerBall(&vec3(0.0f,0.0f,0.0f), &vec3(1.0f,1.0f,1.0f), "PowerUp");
					powerUp->setPos(pos);
					((AABB*)powerUp->getBoundingVolume())->calculateAngle(false, false);
					m_pPUObservable->broadcastRebirth(powerUp);
					m_powerUps.push_back(powerUp);
				}
				break;
			case SMALLERPAD:
				{
					PUSmallerPad* powerUp = new PUSmallerPad(&vec3(0.0f,0.0f,0.0f), &vec3(1.0f,1.0f,1.0f), "PowerUp");
					powerUp->setPos(pos);
					((AABB*)powerUp->getBoundingVolume())->calculateAngle(false, false);
					m_pPUObservable->broadcastRebirth(powerUp);
					m_powerUps.push_back(powerUp);
				}
				break;

			default:
				break;
			}
		}
	}
}

int Game::random()
{
	int r = rand() % 10000;
	if(r < 3000)
		return FASTERBALL;
	else if(r < 4000)
		return SLOWERBALL;
	else if(r < 6000)
		return BIGGERPAD;
	else if(r < 7000)
		return SMALLERPAD;
	else 
		return STICKYPAD;
}

void Game::loadSounds()
{
	m_pSoundList.resize(2);
	m_soundManager.init();

	m_pSoundList.at(COLLISION) = m_soundManager.loadSound("Sounds/thunder.wav", false);
	m_pSoundList.at(BACKGROUND) = m_soundManager.loadSound("Sounds/mainbackground.mp3", true);
}

void Game::setScreenBorders(vec4 p_screenBorders)
{
	m_screenBorders = p_screenBorders;
}

void Game::resetBall(PlayField* pf)
{
	((Ball*)m_pBall)->setStuck(true);
	((Pad*)m_pPad)->setSavedVector(vec3(0.0f, 4.0f, 0.0f) * ((Pad*)m_pPad)->getScale());

	((Ball*)m_pBall)->setInternalPosition( *m_pPad->getBoundingVolume()->getPosition() + 
		((Pad*)m_pPad)->getSavedVector(),
		pf->getOriginalPosition(), pf->getRightDir(), pf->getDownDir() );
	((Ball*)m_pBall)->updateBoundingVolume(pf->getOriginalPosition(),pf->getRightDir(),pf->getDownDir());
}

int Game::getRemainingLives()
{
	return m_player.lives;
}

int Game::getScore()
{
	return m_player.highscore;
}


void Game::setWinScreen(WinScreen* p_winScene)
{
	m_winScene = p_winScene;
}

void Game::setDeathScreen(DeathScreen* p_deathScene)
{
	m_deathScene = p_deathScene;
}

bool Game::paused()
{
	return m_paused;
}