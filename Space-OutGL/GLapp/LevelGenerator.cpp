#include "Header/LevelGenerator.h"

LevelGenerator::LevelGenerator()
{
	m_posXOffsetFB	= 0;
	m_posXOffsetL	= 0;
	m_posXOffsetR	= 0;
	m_posZOffsetLR	= 0;
	m_posZOffsetF	= 0;
	m_posZOffsetB	= 0;
	m_posYOffset	= 0;
}

LevelGenerator::~LevelGenerator()
{
	m_posXOffsetFB	= 0;
	m_posXOffsetL	= 0;
	m_posXOffsetR	= 0;
	m_posZOffsetLR	= 0;
	m_posZOffsetF	= 0;
	m_posZOffsetB	= 0;
	m_posYOffset	= 0;
	m_blockCountX	= 0;
	m_blockCountY	= 0;
	m_offsetLimit	= 0;
	m_fieldSize.x	= 0;
	m_fieldSize.y	= 0;
}

void LevelGenerator::loadFile(string p_fileName)
{
	string line;
	char* lineChar;
	int offset = 0;
	fstream file;
	file.open(p_fileName.c_str());
	
	if(file.is_open())
	{
		//X-Axis
		getline(file, line);
		m_blockCountX = stringToNumber(line);
		//Y-Axis
		getline(file, line);
		m_blockCountY = stringToNumber(line);
		m_offsetLimit = (m_blockCountX * m_blockCountY) - m_blockCountX;
		//Calculate offset based of X, Y
		calcOffsets();

		getline(file,line);
		m_fieldSize.x = (float)stringToNumber(line);
		getline(file,line);
		m_fieldSize.y = (float)stringToNumber(line);

		//Rest of file
		m_loadedData.resize(m_blockCountX*m_blockCountY);
		getline(file,line);

		for(int i = 0; i < 4; i++)
		{
			getline(file, line);
			offset = 0;
			while(line.length()>1)
			{
				unsigned int size = line.length() + 1;
				lineChar = new char[size];
				strcpy(lineChar, line.c_str());
				char* token;
				token = strtok(lineChar, ";");
				int looplength = (int)(offset + size * 0.5f);

				for (int j = offset; j < looplength ; j++)
				{
					if (token != NULL)
					{
						int a = 0;
						a += atoi(token);
						m_loadedData.at(j) = a;
						token = strtok( NULL, ";");
					}
				}
				if(offset < m_offsetLimit)
					offset += size/2;
				if(!file.eof())
					getline(file, line);
				else 
					line = ' ';
			}
			m_blockLists.push_back(vector<ABlock*>());
			createBlocks((FACE)i);
		}
	}
	
}

void LevelGenerator::createBlocks(FACE p_face)
{
	int offset = 0;
	int row = 0;
	for (int i = 0; i < m_blockCountY; i++)
	{
		for (int j = offset; j < (m_blockCountX + offset); j++)
		{
			
			switch (m_loadedData[j])
			{
			case 0:
				break;
			case 1:
				addBlockToList(i, row, p_face, BLOCK);
				break;

			case 2:
				addExpBlockToList(i, row, p_face, EXPBLOCK);
				break;

			case 3:
				addHardBlockToList(i, row, p_face, HARDBLOCK);
				break;

			default:
				break;
			}
			row++;
		}
		offset += m_blockCountX;
		row = 0;
	}
}

vector<ABlock*> LevelGenerator::getBlockList(int p_list)
{
	return m_blockLists.at(p_list);
}

void LevelGenerator::addBlockToList(int i, int row, FACE p_face, unsigned int p_blockType)
{
	vec3 temp = vec3(0);
	switch(p_face)
	{
		case FRONT:
			m_blockLists.at(0).push_back(new Block(&temp, &temp, p_blockType, "Block", vec2(row, i)));
			break;		
		case BACK:		
			m_blockLists.at(1).push_back(new Block(&temp, &temp, p_blockType, "Block", vec2(row, i)));
			break;		
		case LEFT:		
			m_blockLists.at(2).push_back(new Block(&temp, &temp, p_blockType, "Block", vec2(row, i)));
			break;		
		case RIGHT:		
			m_blockLists.at(3).push_back(new Block(&temp, &temp, p_blockType, "Block", vec2(row, i)));
			break;
	}
};

void LevelGenerator::addExpBlockToList(int i, int row, FACE p_face, unsigned int p_blockType)
{
	vec3 temp = vec3(0);
	switch(p_face)
	{
		case FRONT:
			m_blockLists.at(0).push_back(new ExpBlock(&temp, &temp, p_blockType, "ExpBlock", vec2(row, i)));
			break;		
		case BACK:		
			m_blockLists.at(1).push_back(new ExpBlock(&temp, &temp, p_blockType, "ExpBlock", vec2(row, i)));
			break;		
		case LEFT:		
			m_blockLists.at(2).push_back(new ExpBlock(&temp, &temp, p_blockType, "ExpBlock", vec2(row, i)));
			break;		
		case RIGHT:		
			m_blockLists.at(3).push_back(new ExpBlock(&temp, &temp, p_blockType, "ExpBlock", vec2(row, i)));
			break;
	}
};

void LevelGenerator::addHardBlockToList(int i, int row, FACE p_face, unsigned int p_blockType)
{
	vec3 temp = vec3(0);
	switch(p_face)
	{
		case FRONT:
			m_blockLists.at(0).push_back(new HardBlock(&temp, &temp, p_blockType, "ExpBlock", vec2(row, i)));
			break;		
		case BACK:		
			m_blockLists.at(1).push_back(new HardBlock(&temp, &temp, p_blockType, "ExpBlock", vec2(row, i)));
			break;		
		case LEFT:		
			m_blockLists.at(2).push_back(new HardBlock(&temp, &temp, p_blockType, "ExpBlock", vec2(row, i)));
			break;		
		case RIGHT:		
			m_blockLists.at(3).push_back(new HardBlock(&temp, &temp, p_blockType, "ExpBlock", vec2(row, i)));
			break;
	}
};

int LevelGenerator::stringToNumber(string p_number)
{
	stringstream ss(p_number);
	int result;
	ss >> result;
	return result;
}

void LevelGenerator::calcOffsets()
{
	//Front, back X
	m_posXOffsetFB = 0.0f;
	//Left X
	m_posXOffsetL = -g_bvSize.x;
	//Right X
	m_posXOffsetR = g_bvSize.x * 2.0f * m_blockCountX + g_bvSize.z;
	//Left, Right Z
	m_posZOffsetLR = g_bvSize.x;
	//Front Z
	m_posZOffsetF = 0.0f;
	//Back Z
	m_posZOffsetB = g_bvSize.x * 2.0f * m_blockCountX + g_bvSize.z;
	//All Y
	m_posYOffset = 30.0f;
}	  
	  
	  
	  
	  
	  

vec2 LevelGenerator::getFieldSize()
{
	return m_fieldSize;
}

vec2 LevelGenerator::getNrBlocks()
{
	return vec2(m_blockCountX, m_blockCountY);
}