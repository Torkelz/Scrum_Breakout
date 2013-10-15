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
#include "Difficulties.h"

using namespace glm;

class Observer;
class Object;

enum DIFFICULTIES
{
	EASY,
	NORMAL,
	HARD
};

enum SOUNDS
{
	BACKGROUND,
	COLLISION,
	POWERUP,
	POWERDOWN,
	DEATH
};

struct SPlayer
{
	unsigned short lives;
	unsigned int highscore;
	float multipler;

	SPlayer(){}

	SPlayer(unsigned short p_lives, float p_mul)
	{
		lives = p_lives;
		highscore = 0;
		multipler = p_mul;
	}
};

class Game
{
public:
	Game();
	~Game();

	void					init(PUObserver* p_pPUObserver, DIFFICULTIES p_diff);

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
	void					setScreenBorders(vec4 p_screenBorder);
private:
	SInitDataDifficulties	m_sDiffData;
	SPlayer					m_player;

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

	vec4					m_screenBorders;
};

#endif	GAME_H