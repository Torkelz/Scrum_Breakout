#ifndef LEVELGENERATOR_H
#define LEVELGENERATOR_H

#include <string>
#include <fstream>
#include <sstream>
#include "Block.h"
#include <glm.hpp>

using namespace glm;
using namespace std;

enum FACE
{
	FRONT,
	BACK,
	LEFT,
	RIGHT
};

struct BlockLists
{
	vector<ABlock*>	m_blocksFront;
	vector<ABlock*>	m_blocksBack;
	vector<ABlock*>	m_blocksLeft;
	vector<ABlock*>	m_blocksRight;
};

class LevelGenerator
{
public:
	LevelGenerator();
	~LevelGenerator();

	void loadFile(string p_fileName);
	void createBlocks(FACE p_face);
	BlockLists* getBlocks();


private:
	int				m_loadedData[200];
	BlockLists		m_blockLists;

	void addBlockToList(int i, int row, FACE p_face);
};

#endif 
