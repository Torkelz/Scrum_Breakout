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

//struct BlockLists
//{
//	vector<ABlock*>	m_blocksFront;
//	vector<ABlock*>	m_blocksBack;
//	vector<ABlock*>	m_blocksLeft;
//	vector<ABlock*>	m_blocksRight;
//};

class LevelGenerator
{
public:
	LevelGenerator();
	~LevelGenerator();

	void loadFile(string p_fileName);
	void createBlocks(FACE p_face);
	vector<vector<ABlock*>>* getBlocks();
	

private:
	vector<int>		m_loadedData;
	int				m_blockCountX;
	int				m_blockCountY;
	int				m_offsetLimit;
	vector<vector<ABlock*>> m_blockLists;

	float			m_posXOffsetFB; //X-Front, X-Back
	float			m_posXOffsetL;	//X-Left
	float			m_posXOffsetR;	//X-Right
	float			m_posZOffsetLR; //Z-Left, Z-Right
	float			m_posZOffsetF;	//Z-Front
	float			m_posZOffsetB;	//Z-Back
	float			m_posYOffset;	//Y-offset all	

	int				stringToNumber(string p_number);
	void			addBlockToList(int i, int row, FACE p_face);
	void			calcOffsets();
};

#endif 
