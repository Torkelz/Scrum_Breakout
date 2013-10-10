#ifndef GAME_H
#define GAME_H

#define CLIENTWIDTH		800
#define CLIENTHEIGHT	600

#include <glm.hpp>
#include <gtc\matrix_transform.hpp>
#include <vector>
#include "LevelGenerator.h"
#include "PlayField.h"
#include "PowerUpInclude.h"
#include "PUObservable.h"
#include "FSound.h"
#include "Camera.h"

using namespace glm;

class Observer;
class Object;

enum SOUNDS
{
	BACKGROUND,
	COLLISION,
	POWERUP,
	POWERDOWN,
	DEATH
};

class Game
{
public:
	Game();
	~Game();

	void					init(PUObserver* p_pPUObserver);

	void					update(float p_screenWidth, float p_dt);
	void					leftMouseClick( vec2 p_mousePosition );
	void					rightMouseClick( vec2 p_mousePosition );
	void					mouseMove( vec2 p_mousePosition );
	void					keyEvent( unsigned short p_key );
	Observer*				getObserver();
	Object*					getPad();
	vector<ABlock*>*		getBlocks(int p_list);
	Object*					getBall();
	PlayField*				getField(int p_id);
	PlayField*				getActiveField();
	unsigned int			getActiveFieldNr();

	Camera*					getCamera();
private:
	void					loadSounds();
	FSound					m_soundManager;
	float					m_newVolume;

	vector<FMOD::Sound*>	m_pSoundList;
	


	void					powerUpSpawn(vec3 pos);
	int					    random();
	void					powerUpCheck(int i);

	Observer*				m_pObserver;
	// ## POWER UP OBSERVABLE ##
	PUObservable*			m_pPUObservable;
	Object*					m_pPad;
	Object*					m_pBall;
	vector<vector<ABlock*>>*m_pBlocks;
	LevelGenerator			m_loadLevel;
	vector<PowerUp*>		m_powerUps;
	Camera*					m_pCamera;

	static const unsigned int	m_nrPlayFields = 4;
	vec3					m_originWorld;
	unsigned int			m_activePlayField;
	int			m_activePlayFieldNext;
	PlayField*				m_playFields[m_nrPlayFields];
	float					m_counter;
	int						m_padCounter;
	int						m_wallCounter;
	bool					m_padCrash;
	bool					m_wallCrash;
};

#endif	GAME_H