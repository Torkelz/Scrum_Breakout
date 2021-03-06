#ifndef LEVELGENERATOR_H
#define LEVELGENERATOR_H

#include <string>
#include <fstream>
#include <sstream>
#include "Block.h"
#include "ExpBlock.h"
#include "HardBlock.h"
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

class LevelGenerator
{
public:
	LevelGenerator();
	~LevelGenerator();

	void loadFile(string p_fileName);
	void createBlocks(FACE p_face);
	vector<ABlock*> getBlockList(int p_list);

	vec2			getFieldSize();
	vec2			getNrBlocks();

private:
	vector<int>		m_loadedData;
	int				m_blockCountX;
	int				m_blockCountY;
	int				m_offsetLimit;
	vector<vector<ABlock*>> m_blockLists;

	vec2			m_fieldSize;

	float			m_posXOffsetFB; //X-Front, X-Back
	float			m_posXOffsetL;	//X-Left
	float			m_posXOffsetR;	//X-Right
	float			m_posZOffsetLR; //Z-Left, Z-Right
	float			m_posZOffsetF;	//Z-Front
	float			m_posZOffsetB;	//Z-Back
	float			m_posYOffset;	//Y-offset all	

	int				stringToNumber(string p_number);
	void			addBlockToList(int i, int row, FACE p_face, unsigned int p_blockType);
	void			addExpBlockToList(int i, int row, FACE p_face, unsigned int p_blockType);
	void			addHardBlockToList(int i, int row, FACE p_face, unsigned int p_blockType);
	void			calcOffsets();
};

#endif 
