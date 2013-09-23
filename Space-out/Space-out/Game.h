#ifndef GAME_H
#define GAME_H

#include <glm.hpp>
#include <gtc\matrix_transform.hpp>
#include <vector>
#include "LevelGenerator.h"

using namespace glm;

class Observer;
class Object;
class ABlock;
class Block;

class Game
{
public:
	Game();
	~Game();

	void					init();

	void		update(float p_screenWidth);
	void		leftMouseClick( vec2 p_mousePosition );
	void		rightMouseClick( vec2 p_mousePosition );
	void		mouseMove( vec2 p_mousePosition );
	void					keyEvent( unsigned short p_key );
	Observer*				getObserver();
	Object*					getPad();
	vector<ABlock*>*		getBlocks(int p_list);
	Object*		getBall();
private:
	Observer*				m_pObserver;
	Object*					m_pPad;
	Object*					m_pBall;
	vector<vector<ABlock*>>*m_pBlocks;
	LevelGenerator			m_loadLevel;
};

#endif	GAME_H