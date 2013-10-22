#ifndef GAME_H
#define GAME_H

#define CLIENTWIDTH		800
#define CLIENTHEIGHT	600

#include <glm.hpp>
#include <gtc\matrix_transform.hpp>
#include "..\glm\gtx\rotate_vector.hpp"
#include <vector>
#include "LevelGenerator.h"
#include "PlayField.h"
#include "PowerUpInclude.h"
#include "PUObservable.h"
#include "FSound.h"
#include "Camera.h"
#include "Difficulties.h"
#include "Ball.h"

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

struct Borders
{
	vec3	pos;
	Borders(vec3 p_pos)
	{
		pos		= p_pos;
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
	int						getNrBalls();
	Object*					getBall(int p);
	PlayField*				getField(int p_id);
	PlayField*				getActiveField();
	unsigned int			getActiveFieldNr();
	int						getNrofBorders();
	vector<Borders>*		getBorders();
	Camera*					getCamera();
	void					setScreenBorders(vec4 p_screenBorder);
	int						getRemainingLives();
	int						getScore();
	
	//borders
	void					addBorders();
private:
	vector<int>				findBlockWhoWILLDIEByExplosion(int i);
	vector<Borders>			m_borderList;
	void					loadSounds();
	void					powerUpSpawn(vec3 pos);
	int					    random();
	void					powerUpCheck(int i);
	void					resetBall(PlayField* pf);
	void					spawnBalls(float p_sAngle, float p_eAngle, unsigned int p_numBalls, Ball* p_ball);

	Observer*				m_pObserver;
	// ## POWER UP OBSERVABLE ##
	PUObservable*			m_pPUObservable;
	Object*					m_pPad;
	vector<Object*>			m_pBall;
	vector<vector<ABlock*>>*m_pBlocks;
	LevelGenerator			m_loadLevel;
	vector<PowerUp*>		m_powerUps;
	Camera*					m_pCamera;

	static const unsigned int	m_nrPlayFields = 4;
	vec3					m_originWorld;
	unsigned int			m_activePlayField;
	int						m_activePlayFieldNext;
	PlayField*				m_playFields[m_nrPlayFields];
	float					m_counter;

	// EXP BLOCK
	vec2					m_nrOfBlocksXY;
	vector<int>	      m_neighbourBlockIndex;

	vec4					m_screenBorders;
	
	FSound					m_soundManager;
	float					m_newVolume;
	vector<FMOD::Sound*>	m_pSoundList;

	SInitDataDifficulties	m_sDiffData;
	SPlayer					m_player;
};

#endif	GAME_H