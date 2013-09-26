#ifndef GAME_H
#define GAME_H

#include <glm.hpp>
#include <gtc\matrix_transform.hpp>
#include <vector>
#include "LevelGenerator.h"
#include "PlayField.h"


using namespace glm;

class Observer;
class Object;

class Game
{
public:
	Game();
	~Game();

	void					init();

	void		update(float p_screenWidth, float p_dt);
	void		leftMouseClick( vec2 p_mousePosition );
	void		rightMouseClick( vec2 p_mousePosition );
	void		mouseMove( vec2 p_mousePosition );
	void					keyEvent( unsigned short p_key );
	Observer*				getObserver();
	Object*					getPad();
	vector<ABlock*>*		getBlocks(int p_list);
	Object*					getBall();
	PlayField*				getField(int p_id);
private:
	Observer*				m_pObserver;
	Object*					m_pPad;
	Object*					m_pBall;
	vector<vector<ABlock*>>*m_pBlocks;
	LevelGenerator			m_loadLevel;

	static const unsigned int		m_nrPlayFields = 4;
	vec3					m_originWorld;

	unsigned int					m_activePlayField;		

	PlayField*				m_playFields[m_nrPlayFields];
	
};

#endif	GAME_H