#ifndef GAME_H
#define GAME_H

#define CLIENTWIDTH		800
#define CLIENTHEIGHT	600

#include <glm/glm.hpp>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include "LevelGenerator.h"
#include "PlayField.h"
#include "ABlock.h"
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

struct Borders
{
        vec3        pos;
        Borders(vec3 p_pos)
        {
                pos                = p_pos;
        }
};

class Game
{
public:
	Game(){}
	Game(GLFWwindow* p_hMainWnd);
	~Game();

	void					init(PUObserver* p_pPUObserver, DIFFICULTIES p_diff);

	void					update(float p_screenWidth, float p_dt);
	void					keyEvent( unsigned short p_key );
	Observer*				getObserver();
	Object*					getPad();
	vector<Borders>*		getBorders();
	Object*					getBall(int p);

	Camera*					getCamera();
	void					setScreenBorders(vec4 p_screenBorder);
	PlayField*				getField(int p_id);
	PlayField*				getActiveField();
	unsigned int			getActiveFieldNr();

private:
	void					loadSounds();
	void					powerUpSpawn(vec3 pos);
	void 					addBorders();
	int					    random();
	void					powerUpCheck(int i);
	void					resetBall(PlayField* pf);
	float					deltaSpeed;
	float					globalTime;
	Observer*				m_pObserver;
	// ## POWER UP OBSERVABLE ##
	PUObservable*			m_pPUObservable;
	Object*					m_pPad;
	vector<Object*>         m_pBall;
	LevelGenerator			m_loadLevel;
	vector<PowerUp*>		m_powerUps;
	ABlock*					m_block;
	Camera*					m_pCamera;

	static const unsigned int	m_nrPlayFields = 4;
	vec3					m_originWorld;
	unsigned int			m_activePlayField;
	int						m_activePlayFieldNext;
	PlayField*				m_playFields[m_nrPlayFields];
	float					m_counter;
	int						m_padCounter;
	int						m_wallCounter;
	bool					m_padCrash;
	bool					m_wallCrash;
	GLFWwindow*				m_hMainWnd;
	vec4					m_screenBorders;
	
	vector<Borders>			m_borderList;

	//FSound					m_soundManager;
	float					m_newVolume;
	//vector<FMOD::Sound*>	m_pSoundList;

	SInitDataDifficulties	m_sDiffData;
	SPlayer					m_player;

};

#endif	/* GAME_H */