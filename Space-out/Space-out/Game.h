#ifndef GAME_H
#define GAME_H

#include "Vector2.h"
#include <vector>
#include "LevelGenerator.h"

class Observer;
class Object;
class Pad;
class ABlock;
class Block;

class Game
{
public:
	Game();
	~Game();

	void					init();

	void					update();
	void					leftMouseClick( Vector2 p_mousePosition );
	void					rightMouseClick( Vector2 p_mousePosition );
	void					mouseMove( Vector2 p_mousePosition );
	void					keyEvent( unsigned short p_key );
	Observer*				getObserver();
	Object*					getPad();
	vector<ABlock*>*		getBlocks(int p_list);

private:
	Observer*				m_pObserver;
	Object*					m_pPad;
	vector<vector<ABlock*>>*m_pBlocks;
	LevelGenerator			m_loadLevel;
};

#endif	GAME_H