#include "Header/Game.h"
#include "Header/Observer.h"
#include "Header/Pad.h"
#include "Header/Ball.h"
#include "Header/Block.h"
Game::Game(GLFWwindow* p_hMainWnd):m_hMainWnd(p_hMainWnd)
{
	deltaSpeed = 150;
}
Game::~Game(){}

void Game::init(PUObserver* p_pPUObserver, DIFFICULTIES p_diff)
{
	Difficulties diff = Difficulties();
	diff.setInitValues(p_diff);
	m_sDiffData = diff.getDifficultyValues();

	m_player = SPlayer((short)m_sDiffData.lives, m_sDiffData.multiplier);

	m_pObserver = new Observer(this);
	vec3 temp = vec3(0.0f, 125.0f, 0.0f);
	m_pPad		= new Pad(&temp, &temp, "Pad", 1.4);//m_sDiffData.padStartSize);
	temp = vec3(50.0f, 100.0f, 0.0f);
	vec3 tempSpeed = vec3(10.0f, -20.0f, 0.0f);
	m_pBall.push_back(new Ball(&temp, &temp, "Ball", tempSpeed));//m_sDiffData.ballStartSpeed);
	m_loadLevel = LevelGenerator();
	m_loadLevel.loadFile("/home/bthp0000/Desktop/GitHub/Git/Space-OutGL/GLapp/Levels/Level2.txt");
//
//	m_newVolume = 0.5f;

	m_activePlayField = 0;
	m_activePlayFieldNext = m_activePlayField;

	m_originWorld = vec3(0.f,0.f,0.f);

	vec2 size = m_loadLevel.getFieldSize();
	size *= 0.5f;
	vec3 startPosition(m_originWorld.x+size.x, m_originWorld.y + size.y, m_originWorld.z + size.x);
	size *= 2;
	float angle = 0;

	//Create Fields
	for(unsigned int i = 0; i < m_nrPlayFields; i++, angle += 90)
	{
		m_playFields[i] = new PlayField(startPosition, angle, size);
	}

	for(unsigned int i = 0; i < m_nrPlayFields; i++)
	{
		int t = i % 2;
		m_playFields[i]->init(m_loadLevel.getBlockList(i), m_loadLevel.getNrBlocks(), t);
	}
//
//	//Set ball bounding box
	PlayField* pf = m_playFields[m_activePlayField];
	((Ball*)m_pBall.back())->init(pf->getOriginalPosition(), pf->getRightDir(), pf->getDownDir());
	//resetBall(pf);
//
	m_pCamera = new Camera();
	m_pCamera->init(pf->calculateCameraCenterPos());
	m_pCamera->setViewMatrix();
	m_pCamera->createProjectionMatrix(PI*0.25f,(float)CLIENTWIDTH/CLIENTHEIGHT, 1.0f, 500.0f);
	//m_pCamera->setYaw(m_activePlayField);
//
	m_loadLevel.~LevelGenerator();
//	m_pPUObservable = new PUObservable();
//	m_pPUObservable->addSubscriber(p_pPUObserver);
//
//	loadSounds();
//	m_soundManager.play(m_pSoundList.at(BACKGROUND), 0);
//	m_soundManager.setVolume(0.0f, 0);
     addBorders();
//    int tempPowerUpSize = m_powerUps.size() - 1;
//    for(int i = tempPowerUpSize; i >= 0; i--)
//    {
//            m_pPUObservable->broadcastDeath(i);
//            m_powerUps.erase(m_powerUps.begin() + i);
//    }
}

void Game::update(float p_screenWidth, float p_dt)
{
//	  m_soundManager.update();
//
	                m_pCamera->updateCameraPos(p_dt);
	                globalTime = p_dt;
	                if(m_pCamera->timeToChange())
	                {
	                        m_activePlayField = m_activePlayFieldNext;
//	                        int tempPowerUpSize = m_powerUps.size() - 1;
//	                        for(int i = tempPowerUpSize; i >= 0; i--)
//	                        {
//	                                m_pPUObservable->broadcastDeath(i);
//	                                m_powerUps.erase(m_powerUps.begin() + i);
//	                        }
	                }
//
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
	                                                        //m_soundManager.play(m_pSoundList[COLLISION], 1);
	                                                        //m_soundManager.setVolume(0.35f, 1);
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

	                                                if( bv->collide(m_pBall.at(b)->getBoundingVolume()) && !((Ball*)m_pBall.at(b))->getWallCrash())
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
//	                                        powerUpSpawn(*m_playFields[m_activePlayField]->getBlock(i)->getPos());

//	                                        if(m_playFields[m_activePlayField]->getBlock(i)->getBlockType() == EXPBLOCK)
//	                                        {
//	                                                m_neighbourBlockIndex = findBlockWhoWILLDIEByExplosion(i);
//	                                                for(int exp = m_neighbourBlockIndex.size() - 1; exp >= 0; exp--)
//	                                                {
//	                                                        m_playFields[m_activePlayField]->getBlock(m_neighbourBlockIndex.at(exp))->decreaseHP(10);
//	                                                }
//	                                        }
	                                        m_player.highscore += 10;
	                                        m_playFields[m_activePlayField]->deleteBlock(i);
	                                        break;
	                                }
	                        }
//	                        //Pad vs Borders NEEDS FINE TUNING
	                        for(unsigned int i = 0; i < m_playFields[m_activePlayField]->getNrBorders()-2; i++)
	                        {
	                                AABB* bv = (AABB*)(m_playFields[m_activePlayField]->getCollisionBorder(i));

	                                if( bv->collide(m_pPad->getBoundingVolume()))
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
//
//	                        padPos = ((Pad*)m_pPad)->getRealPosition();
//	                        for(unsigned int i = 0; i < m_powerUps.size(); i++)
//	                        {
//	                                // Update position for power ups.
//	                                mat4 powTranslation;// = translate(mat4(1.0f), vec3(m_powerUps.at(i)->getPos()->x, m_powerUps.at(i)->getPos()->y, t_pos.z));
//	                                if (m_activePlayField == 0 || m_activePlayField == 2)
//	                                        powTranslation = translate(mat4(1.0f), vec3(m_powerUps.at(i)->getPos()->x, m_powerUps.at(i)->getPos()->y, padPos.z)); // Translate powerup
//	                                else
//	                                        powTranslation = translate(mat4(1.0f), vec3(padPos.x, m_powerUps.at(i)->getPos()->y, m_powerUps.at(i)->getPos()->z));
//
//	                                m_powerUps.at(i)->update(p_dt, powTranslation);
//	                                AABB* bv = (AABB*)(m_powerUps.at(i)->getBoundingVolume());
//
//	                                if( bv->collide(m_pPad->getBoundingVolume()) )
//	                                {
//	                                        //Power Up catch.
//	                                        powerUpCheck(m_powerUps.at(i)->getType());
//	                                        m_player.highscore += 100;
//	                                        m_pPUObservable->broadcastDeath(i);
//	                                        m_powerUps.erase(m_powerUps.begin() + i);
//	                                }
//	                                if( bv->collide(m_playFields[m_activePlayField]->getCollisionBorder(3)))
//	                                {
//	                                        //Power Up missed.
//	                                        m_pPUObservable->broadcastDeath(i);
//	                                        m_powerUps.erase(m_powerUps.begin() + i);
//	                                }
//	                        }
//	                        // ## COLLISION STUFF END ##
//
//	                        if(pf->getBlockListSize() <= 0) // If playfield is empty move the ball to the pad.
//	                        {
//	                                resetBall(pf);
//	                        }
//
//	                        pf = NULL;
//
//	                        // TEST LIVES
//	                        if (m_player.lives <= 0)
//	                        {
//	                                        m_active = false;
//	                                        m_deathScene->setActive(true);
//	                                        m_highScore->addHighScore(m_player.highscore);
//	                        }
//
//	                        int nrOfRemainingBlocks = 0;
//	                        for (int pl = 0; pl < 4; pl++)
//	                        {
//	                                nrOfRemainingBlocks += m_playFields[pl]->getBlockListSize();
//	                        }
//	                        if(nrOfRemainingBlocks <= 0)
//	                        {
//	                                        m_active = false;
//	                                        m_winScene->setActive(true);
//	                                        m_highScore->addHighScore(m_player.highscore);
//	                        }
	                }
//
//	        // SOUND IS OFF HERE REMOVE WHEN NEEDING ZE SOUNDS
//	        m_soundManager.setPauseAll(true);
}

void Game::keyEvent(unsigned short key)
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
	else if(key == 0x51) // Q
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
	else{}
	if(key == 0x1B) //ESC
		glfwSetWindowShouldClose(m_hMainWnd, 1);

	if(key == 0x52) // R
	{
//		PUStickyPad* powerUp = new PUStickyPad(&vec3(0.0f,0.0f,0.0f), &vec3(1.0f,1.0f,1.0f), "PowerUp");
//		powerUp->setPos(vec3(0.0f, m_pPad->getPos()->y, m_pPad->getPos()->z));
//		m_pPUObservable->broadcastRebirth(powerUp);
//		m_powerUps.push_back(powerUp);
	}
	if( key == 2190) // LEFT_ARROW
	{
		((Pad*)m_pPad)->setPos( vec2(m_pPad->getPos()->x+deltaSpeed*globalTime, m_pPad->getPos()->y));
	}
	if( key == 2192) // RIGHT_ARROW
	{
		((Pad*)m_pPad)->setPos( vec2(m_pPad->getPos()->x-deltaSpeed*globalTime, m_pPad->getPos()->y));
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

//void Game::powerUpCheck(int i)
//{
//	switch (i)
//	{
//	case FASTERBALL:
//		((Ball*)m_pBall)->speedUp();
//		break;
//	case SLOWERBALL:
//		((Ball*)m_pBall)->speedDown();
//		break;
//	case BIGGERPAD:
//		((Pad*)m_pPad)->bigger();
//		break;
//	case SMALLERPAD:
//		((Pad*)m_pPad)->smaller();
//		break;
//	case STICKYPAD:
//		((Pad*)m_pPad)->setSticky(true);
//		m_counter = 10.0f;
//	default:
//		break;
//	}
//
//	//Remember to remove the power up outside this function!
//}

//void Game::powerUpSpawn(vec3 pos)
//{
//	if(m_powerUps.size() < 10)
//	{
//		int chance = 25;
//		int r = rand() % 100;
//		// chance for powerups
//		if(r < chance * m_sDiffData.dropRate)
//		{
//			r = rand() % 3;
//			switch (r)
//			{
//			case FASTERBALL:
//				{
//					PUFasterBall* powerUp = new PUFasterBall(&vec3(0.0f,0.0f,0.0f), &vec3(1.0f,1.0f,1.0f), "PowerUp");
//					powerUp->setPos(pos);
//					((AABB*)powerUp->getBoundingVolume())->calculateAngle(false, false);
//					m_pPUObservable->broadcastRebirth(powerUp);
//					m_powerUps.push_back(powerUp);
//				}
//				break;
//			case BIGGERPAD:
//				{
//					PUBiggerPad* powerUp = new PUBiggerPad(&vec3(0.0f,0.0f,0.0f), &vec3(1.0f,1.0f,1.0f), "PowerUp");
//					powerUp->setPos(pos);
//					((AABB*)powerUp->getBoundingVolume())->calculateAngle(false, false);
//					m_pPUObservable->broadcastRebirth(powerUp);
//					m_powerUps.push_back(powerUp);
//				}
//				break;
//			case STICKYPAD:
//				{
//					PUStickyPad* powerUp = new PUStickyPad(&vec3(0.0f,0.0f,0.0f), &vec3(1.0f,1.0f,1.0f), "PowerUp");
//					powerUp->setPos(pos);
//					((AABB*)powerUp->getBoundingVolume())->calculateAngle(false, false);
//					m_pPUObservable->broadcastRebirth(powerUp);
//					m_powerUps.push_back(powerUp);
//				}
//			default:
//				break;
//			}
//		} // Drop chance for powerdowns!
//		else if(r < chance)
//		{
//			r = rand() % 2;
//			switch(r)
//			{
//			case SLOWERBALL:
//				{
//					PUSlowerBall* powerUp = new PUSlowerBall(&vec3(0.0f,0.0f,0.0f), &vec3(1.0f,1.0f,1.0f), "PowerUp");
//					powerUp->setPos(pos);
//					((AABB*)powerUp->getBoundingVolume())->calculateAngle(false, false);
//					m_pPUObservable->broadcastRebirth(powerUp);
//					m_powerUps.push_back(powerUp);
//				}
//				break;
//			case SMALLERPAD:
//				{
//					PUSmallerPad* powerUp = new PUSmallerPad(&vec3(0.0f,0.0f,0.0f), &vec3(1.0f,1.0f,1.0f), "PowerUp");
//					powerUp->setPos(pos);
//					((AABB*)powerUp->getBoundingVolume())->calculateAngle(false, false);
//					m_pPUObservable->broadcastRebirth(powerUp);
//					m_powerUps.push_back(powerUp);
//				}
//				break;
//
//			default:
//				break;
//			}
//		}
//	}
//}

//void Game::loadSounds()
//{
//	m_pSoundList.resize(2);
//	m_soundManager.init();
//
//	m_pSoundList.at(COLLISION) = m_soundManager.loadSound("Sounds/thunder.wav", false);
//	m_pSoundList.at(BACKGROUND) = m_soundManager.loadSound("Sounds/mainbackground.mp3", true);
//}

//void Game::setScreenBorders(vec4 p_screenBorders)
//{
//	m_screenBorders = p_screenBorders;
//}

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
                //                        + m_playFields[playField]->getSize().y * downDir
                //                        + crossDir * 5.f;
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