#include "Game.h"
#include "Observer.h"
#include "Pad.h"

#include "Block.h"
#include <time.h>
#include "SceneInclude.h"

Game::Game(){}
Game::~Game(){}

void Game::init(PUObserver* p_pPUObserver, DIFFICULTIES p_diff)
{
	//Set random seed for powerups
	srand (time(NULL));
	
	m_active = false;
	m_paused = false;

	Difficulties diff = Difficulties();
	diff.setInitValues(p_diff);
	m_sDiffData = diff.getDifficultyValues();

	m_player = SPlayer((short)m_sDiffData.lives, m_sDiffData.multiplier);

	m_pObserver = new Observer(this);
	m_pPad		= new Pad(&vec3(0.0f, 125.0f, 0.0f), &vec3(0.56f, 0.56f, 0.56f), "Pad", m_sDiffData.padStartSize);
	m_pBall.clear();
	m_pBall.push_back( new Ball(&vec3(50.0f, 100.0f, 0.0f), &vec3(0.56f, 0.56f, 0.56f), "Ball", m_sDiffData.ballStartSpeed));
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

	m_nrOfBlocksXY = m_loadLevel.getNrBlocks();	

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
	((Ball*)m_pBall.back())->init(pf->getOriginalPosition(), pf->getRightDir(), pf->getDownDir());
	resetBall(pf);

	m_pCamera = nullptr;
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
	m_soundManager.setVolume(0.3f, 0);
	
	addBorders();
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

		if(!m_pCamera->isCinematic() || (m_pCamera->timeToChange() && m_pCamera->getRunOnce()))
		{
			m_pCamera->setRunOnce(false);
			if(m_counter > 0.0f)
			{
				m_counter -= p_dt;
			}
			else
				((Pad*)m_pPad)->setSticky(false);

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

			for(unsigned int b = 0; b < m_pBall.size();b++)
			{

				if(!((Ball*)m_pBall.at(b))->getStuck())
				{
					((Ball*)m_pBall.at(b))->update(p_dt);
					((Ball*)m_pBall.at(b))->updateBoundingVolume(pf->getOriginalPosition(),pf->getRightDir(),pf->getDownDir());

					//Pad vs Ball
					if(((Pad*)m_pPad)->collide(m_pBall.at(b)->getBoundingVolume()) && !((Ball*)m_pBall.at(b))->getPadCrash())
					{
						if(!((Pad*)m_pPad)->getSticky())
						{
							vec3 tempSpeed = ((AABB*)m_pPad->getBoundingVolume())->findNewDirection(*m_pBall.at(b)->getBoundingVolume()->getPosition(), ((Ball*)m_pBall.at(b))->getSpeed());
					
							tempSpeed.y = -abs(tempSpeed.y);
							((Ball*)m_pBall.at(b))->setSpeed( tempSpeed );
							((Ball*)m_pBall.at(b))->resetPadCrash();
						}
						else
						{
							((Ball*)m_pBall.at(b))->setStuck(true);
							((Pad*)m_pPad)->setSavedVector( ((Ball*)m_pBall.at(b))->getRealPosition()- *m_pPad->getBoundingVolume()->getPosition() );
						}
					}

					// ## BLOCKS ##
					for(int i = 0; i < m_playFields[m_activePlayField]->getBlockListSize();i++)
					{
						AABB* bv = (AABB*)(m_playFields[m_activePlayField]->getBlock(i)->getBoundingVolume());
						if(bv->collide(m_pBall.at(b)->getBoundingVolume()))
						{
							m_soundManager.play(m_pSoundList[COLLISION], 1);
							m_soundManager.setVolume(0.35f, 1);
							vec3 s = ((Ball*)m_pBall.at(b))->getSpeed();
							vec3 tempSpeed = bv->findNewDirection(*m_pBall.at(b)->getBoundingVolume()->getPosition(), s);
							((Ball*)m_pBall.at(b))->setSpeed( tempSpeed );

							m_playFields[m_activePlayField]->getBlock(i)->decreaseHP(1);
					
							if( ( (Ball*)m_pBall.at(b))->getIsExplosive() )
							{
								m_playFields[m_activePlayField]->getBlock(i)->decreaseHP(10);
								m_playFields[m_activePlayField]->getBlock(i)->changeBlockType(EXPBLOCK);
							}

							bv = NULL;
							break;
						}
					}
					// ## END BLOCKS ##

					// ## WALLS ##
					for(unsigned int i = 0; i < m_playFields[m_activePlayField]->getNrBorders(); i++)
					{
						AABB* bv = (AABB*)(m_playFields[m_activePlayField]->getCollisionBorder(i));

						if(  bv->collide(m_pBall.at(b)->getBoundingVolume()) && !((Ball*)m_pBall.at(b))->getWallCrash())
						{
							if (i == 3)
							{
								if(m_pBall.size() == 1)
								{
									resetBall(pf);
									m_player.lives--;
									( (Ball*)m_pBall.at(b))->setExplosive(false);
								}
								else
								{
									m_pBall.erase(m_pBall.begin()+b);
									break;
								}
							}
							vec3 tempSpeed = bv->findNewDirection(*m_pBall.at(b)->getBoundingVolume()->getPosition(), ((Ball*)m_pBall.at(b))->getSpeed());
							((Ball*)m_pBall.at(b))->setSpeed( tempSpeed );
							((Ball*)m_pBall.at(b))->resetWallCrash();
							break;
						}
					}
				}
				else
				{
					((Ball*)m_pBall.at(b))->setInternalPosition( *m_pPad->getBoundingVolume()->getPosition() + ((Pad*)m_pPad)->getSavedVector(),pf->getOriginalPosition(),pf->getRightDir(), pf->getDownDir() );
					((Ball*)m_pBall.at(b))->updateBoundingVolume(pf->getOriginalPosition(),pf->getRightDir(),pf->getDownDir());
				}		
			}//BALL LOOP END
			for(int i = 0; i < m_playFields[m_activePlayField]->getBlockListSize();i++)
			{
				if(m_playFields[m_activePlayField]->getBlock(i)->isDead())
				{
					powerUpSpawn(*m_playFields[m_activePlayField]->getBlock(i)->getPos());
					
					if(m_playFields[m_activePlayField]->getBlock(i)->getBlockType() == EXPBLOCK)
					{
						m_neighbourBlockIndex = findBlockWhoWILLDIEByExplosion(i);
						for(int exp = m_neighbourBlockIndex.size() - 1; exp >= 0; exp--)
						{
							m_playFields[m_activePlayField]->getBlock(m_neighbourBlockIndex.at(exp))->decreaseHP(10);
						}
					}
					m_player.highscore += 10;
					m_playFields[m_activePlayField]->deleteBlock(i);
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
					m_player.highscore += 100;
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

			if(pf->getBlockListSize() <= 0) // If playfield is empty move the ball to the pad.
			{
				resetBall(pf);
			}

			pf = NULL;

			// TEST LIVES
			if (m_player.lives <= 0)
			{
					m_active = false;
					m_deathScene->setActive(true);
					m_highScore->addHighScore(m_player.highscore);
			}

			int nrOfRemainingBlocks = 0;
			for (int pl = 0; pl < 4; pl++)
			{
				nrOfRemainingBlocks += m_playFields[pl]->getBlockListSize();
			}
			if(nrOfRemainingBlocks <= 0)
			{
					m_active = false;
					m_winScene->setActive(true);
					m_highScore->addHighScore(m_player.highscore);
			}
		}
	}

	// SOUND IS OFF HERE REMOVE WHEN NEEDING ZE SOUNDS
	//m_soundManager.setPauseAll(true);
}

vector<int> Game::findBlockWhoWILLDIEByExplosion(int i)
{
	vec2 blockID = m_playFields[m_activePlayField]->getBlock(i)->getBlockID();
	//vector<vec2> blockIndexToExp;
	vector<int>	 neighbourBlockIndex;

	// TOP BLOCK
	if(blockID.y > 0)
	{
		for(int blockCount = i - m_nrOfBlocksXY.x; blockCount < i; blockCount++)
		{
			if(blockCount > 0)
			{
				vec2 temp;
				temp = m_playFields[m_activePlayField]->getBlock(blockCount)->getBlockID();

				if(temp.x == blockID.x && temp.y == blockID.y - 1)
				{
               //if(neighbourBlockIndex[blockCount] != blockCount)
                  neighbourBlockIndex.push_back(blockCount);
					//blockIndexToExp.push_back(m_playFields[m_activePlayField]->getBlock(blockCount)->getBlockID());
					
					break;
				}
			}
		}
	}

	// LEFT BLOCK 
	if(blockID.x > 0)
	{
		if( (i) > 0)
		{
			vec2 temp;
			temp = m_playFields[m_activePlayField]->getBlock(i - 1)->getBlockID();
			if(temp.x == blockID.x - 1 && temp.y == blockID.y)
			{
				//blockIndexToExp.push_back(m_playFields[m_activePlayField]->getBlock(i - 1)->getBlockID());
				neighbourBlockIndex.push_back(i-1);
			}
		}
	}

	//done for destroying THE exploding block ( Block who were hit by ball)
	neighbourBlockIndex.push_back(i);
						
	// RIGHT
	if(blockID.x < m_nrOfBlocksXY.x - 1)
	{
		vec2 temp;
		if( (i+1) < m_playFields[m_activePlayField]->getBlockListSize())
		{
			temp = m_playFields[m_activePlayField]->getBlock(i + 1)->getBlockID();
			if(temp.x == blockID.x + 1 && temp.y == blockID.y)
			{
				//blockIndexToExp.push_back(m_playFields[m_activePlayField]->getBlock(i + 1)->getBlockID());
				neighbourBlockIndex.push_back(i+1);
			}
		}
	}



	// BOTTOM BLOCK
	if(blockID.y < m_nrOfBlocksXY.y - 1)
	{
		for(int blockCount = i + m_nrOfBlocksXY.x; blockCount > i; blockCount--)
		{						
			if(blockCount < m_playFields[m_activePlayField]->getBlockListSize())
			{
				vec2 temp;
				temp = m_playFields[m_activePlayField]->getBlock(blockCount)->getBlockID();

				if(temp.x == blockID.x && temp.y == blockID.y + 1)
				{
					//blockIndexToExp.push_back(m_playFields[m_activePlayField]->getBlock(blockCount)->getBlockID());
					neighbourBlockIndex.push_back(blockCount);
					break;
				}
			}
		}
	}

	return neighbourBlockIndex;
}

void Game::keyEvent(unsigned short key)
{
	if(m_active)
	{
		if(key == 0x41) // A
		{
			((Ball*)m_pBall.front())->setSpeed(vec3(-50.0f, 0.0f, 0.0f) * 3.0f);
		}
		if(key == 0x44) // D
		{
			((Ball*)m_pBall.front())->setSpeed(vec3(50.0f, 0.0f, 0.0f) * 3.0f);
		}
		if(key == 0x57) // W
		{
			((Ball*)m_pBall.front())->setSpeed(vec3(0.0f, -50.0f, 0.0f) * 3.0f);
		}
		if(key == 0x53) // S
		{
			((Ball*)m_pBall.front())->setSpeed(vec3(0.0f, 50.0f, 0.0f) * 3.0f);
		}
		//DEBUG BALL
		if(key == 0x42) // B
		{
			int t = m_pBall.size();
			for(int i = 0; i < t;i++)
			{
			spawnBalls(-180,180,8, (Ball*)m_pBall.at(i));
			}
		}


		if(key == 0x45) // E
		{
			if(!m_pCamera->isCinematic())
			{
				bool ballsCheck = true;
				int apf = m_activePlayField;
				unsigned int npf;
				if(apf - 1 < 0)
					npf = m_nrPlayFields - 1;
				else
					npf = apf - 1;

				ABlock* block = m_playFields[npf]->getLastBlock();
				if(block != nullptr)
				{
					for(unsigned int b = 0; b < m_pBall.size();b++)
					{
						if(((Ball*)m_pBall.at(b))->getRealPosition().y > ((Block*)block)->getBlockVertex().pos.y - 5)//5 offset
						{
							ballsCheck = false;
							break;
						}
					}
				}

				if(ballsCheck)
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
			
					m_pCamera->setRunOnce(true);
				}
				}
			}
			
			if(key == 0x51) // Q
			{
				if(!m_pCamera->isCinematic())
			{
				bool ballsCheck = true;
				unsigned int npf;
				if(m_activePlayField + 1 >= m_nrPlayFields)
					npf = 0;
				else
					npf = m_activePlayField + 1;

				ABlock* block = m_playFields[npf]->getLastBlock();

				if(block != nullptr)
				{
					for(unsigned int b = 0; b < m_pBall.size();b++)
					{
						if(((Ball*)m_pBall.at(b))->getRealPosition().y > ((Block*)block)->getBlockVertex().pos.y - 5)//5 offset
						{
							ballsCheck = false;
							break;
						}
					}
				}

				if(ballsCheck)
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

					m_pCamera->setRunOnce(true);
				}
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
		if( key == 0x46) // F
		{
			PUExplosiveBall* powerUp = new PUExplosiveBall(&vec3(0.0f,0.0f,0.0f), &vec3(1.0f,1.0f,1.0f), "PowerUp");
			powerUp->setPos(vec3(0.0f, m_pPad->getPos()->y, m_pPad->getPos()->z));
			m_pPUObservable->broadcastRebirth(powerUp);
			m_powerUps.push_back(powerUp);
		}
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
		for(unsigned int i = 0; i < m_pBall.size();i++)
		{
			if(((Ball*)m_pBall.at(i))->getStuck())
			{
				((Ball*)m_pBall.at(i))->setStuck(false);
				vec3 tempSpeed = ((AABB*)m_pPad->getBoundingVolume())->findNewDirection(*m_pBall.at(i)->getBoundingVolume()->getPosition(), ((Ball*)m_pBall.at(i))->getSpeed());
				tempSpeed.y = -abs(tempSpeed.y);
				((Ball*)m_pBall.at(i))->setSpeed( tempSpeed );
			}
		}
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

int Game::getNrBalls()
{
	return m_pBall.size();
}

Object* Game::getBall(int p)
{
	return m_pBall.at(p);
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
		for(unsigned int i = 0; i < m_pBall.size(); i++)
		{
			((Ball*)m_pBall.at(i))->speedUp();
		}
		break;
	case SLOWERBALL:
		for(unsigned int i = 0; i < m_pBall.size(); i++)
		{
			((Ball*)m_pBall.at(i))->speedDown();
		}
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
		break;
	case SPLITBALL:
		spawnBalls(-45,45,2, (Ball*)m_pBall.front());
		break;
	case SCATTERBALL:
		if(((Ball*)m_pBall.front())->getStuck())
			spawnBalls(-85,85,8,(Ball*)m_pBall.front());
		else
			spawnBalls(-180,180,8,(Ball*)m_pBall.front());
		break;
	case EXPLOSIVEBALL:
		((Ball*)m_pBall.front())->setExplosive(true);
		break;
	case ONEUP:
		m_player.lives++;
		break;
	case ONEDOWN:
		m_player.lives--;
		break;
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
			r = rand() % POWERUPTYPECOUNT;
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
				break;
			case SPLITBALL:
			{
				PUSplitBall* powerUp = new PUSplitBall(&vec3(0.0f,0.0f,0.0f), &vec3(1.0f,1.0f,1.0f), "PowerUp");
				powerUp->setPos(pos);
				((AABB*)powerUp->getBoundingVolume())->calculateAngle(false, false);
				m_pPUObservable->broadcastRebirth(powerUp);
				m_powerUps.push_back(powerUp);
			}
			break;
			case SCATTERBALL:
			{
				PUScatterBall* powerUp = new PUScatterBall(&vec3(0.0f,0.0f,0.0f), &vec3(1.0f,1.0f,1.0f), "PowerUp");
				powerUp->setPos(pos);
				((AABB*)powerUp->getBoundingVolume())->calculateAngle(false, false);
				m_pPUObservable->broadcastRebirth(powerUp);
				m_powerUps.push_back(powerUp);
			}
			break;
			case EXPLOSIVEBALL:
				{
					PUExplosiveBall* powerUp = new PUExplosiveBall(&vec3(0.0f,0.0f,0.0f), &vec3(1.0f,1.0f,1.0f), "PowerUp");
					powerUp->setPos(pos);
					((AABB*)powerUp->getBoundingVolume())->calculateAngle(false, false);
					m_pPUObservable->broadcastRebirth(powerUp);
					m_powerUps.push_back(powerUp);
				}
				break;
			case ONEUP:
				{
					PUOneUp* powerUp = new PUOneUp(&vec3(0.0f,0.0f,0.0f), &vec3(1.0f,1.0f,1.0f), "PowerUp");
					powerUp->setPos(pos);
					((AABB*)powerUp->getBoundingVolume())->calculateAngle(false, false);
					m_pPUObservable->broadcastRebirth(powerUp);
					m_powerUps.push_back(powerUp);
				}
				break;
			default:
				break;
			}
		} // Drop chance for powerdowns!
		else if(r < chance)
		{
			r = rand() % (POWERDOWNTYPECOUNT - (POWERUPTYPECOUNT+1)) + POWERUPTYPECOUNT+1 ;
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
			case ONEDOWN:
				{
					PUOneDown* powerUp = new PUOneDown(&vec3(0.0f,0.0f,0.0f), &vec3(1.0f,1.0f,1.0f), "PowerUp");
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
	for(unsigned int i = 0; i < m_pBall.size(); i++)
	{

		((Ball*)m_pBall.at(i))->setStuck(true);
		((Pad*)m_pPad)->setSavedVector(vec3(0.0f, 4.0f, 0.0f) * ((Pad*)m_pPad)->getScale());

		((Ball*)m_pBall.at(i))->setInternalPosition( *m_pPad->getBoundingVolume()->getPosition() + 
			((Pad*)m_pPad)->getSavedVector(),
			pf->getOriginalPosition(), pf->getRightDir(), pf->getDownDir() );
		((Ball*)m_pBall.at(i))->updateBoundingVolume(pf->getOriginalPosition(),pf->getRightDir(),pf->getDownDir());
	}
}

void Game::addBorders()
{
	//get playfield orgpos... 
	//pos for 3 borders... 
	//top, bot, left side... 
	//add them to the vector of borders... 

		//vec3 lol = m_playFields[playfeildnr]->getOriginalPosition();

	for(int playField = 0; playField < 4; playField++)
	{
		vec3 rightDir = m_playFields[playField]->getRightDir();
		vec3 downDir = m_playFields[playField]->getDownDir();
		vec3 crossDir = cross(rightDir, downDir);

		vec3 top = m_playFields[playField]->getOriginalPosition() + (m_playFields[playField]->getSize().x * 0.5f * rightDir)
					- downDir * 5.f
					+ crossDir * 5.f;
		m_borderList.push_back(Borders(top));

		//vec3 bottom = m_playFields[playField]->getOriginalPosition() + (m_playFields[playField]->getSize().x * 0.5f * rightDir)
		//			+ m_playFields[playField]->getSize().y * downDir
		//			+ crossDir * 5.f;
		//m_borderList.push_back(Borders(bottom));
	}

	for(int playField = 0; playField < 4; playField++)
	{
		vec3 rightDir = m_playFields[playField]->getRightDir();
		vec3 downDir = m_playFields[playField]->getDownDir();
		vec3 crossDir = cross(rightDir, downDir);

		vec3 center = m_playFields[playField]->getOriginalPosition() - 5.f * rightDir
			+ m_playFields[playField]->getSize().y * 0.5f * downDir
			+ crossDir * 5.f;
		m_borderList.push_back(Borders(center));
	}
}
int Game::getNrofBorders()
{
	return (int)m_borderList.size();
}
int Game::getRemainingLives()
{
	return m_player.lives;
}
int Game::getScore()
{
	return m_player.highscore;
}

vector<Borders>* Game::getBorders()
{
        return &m_borderList;
}

void Game::spawnBalls(float p_sAngle, float p_eAngle, unsigned int p_numBalls, Ball* p_ball)
{
        mat4 rot;
        float stepAngle = (p_eAngle - p_sAngle)/(p_numBalls-1);
        vec3 playfieldOrtho = m_playFields[m_activePlayField]->getOrthoDir();


        vec3 pos, ospeed,speed;
        pos = p_ball->getRealPosition();
        ospeed = p_ball->getSpeed();


        for(unsigned int i = 0; i < p_numBalls; i++)
        {
                speed = glm::rotate(ospeed, p_sAngle+(stepAngle*i),vec3(0,0,1));


                m_pBall.push_back(new Ball(&pos, &vec3(0.56f, 0.56f, 0.56f), "Ball", speed));
                ((Ball*)m_pBall.back())->setInternalPosition(pos, m_playFields[m_activePlayField]->getOriginalPosition(), 
                                                                                m_playFields[m_activePlayField]->getRightDir(), 
                                                                                m_playFields[m_activePlayField]->getDownDir());
                ((Ball*)m_pBall.back())->init(m_playFields[m_activePlayField]->getOriginalPosition(), 
                                                                                m_playFields[m_activePlayField]->getRightDir(), 
                                                                                m_playFields[m_activePlayField]->getDownDir());
        }
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

void Game::setHighScore(HighScore* p_pHighScore)
{
	m_highScore = p_pHighScore;
}