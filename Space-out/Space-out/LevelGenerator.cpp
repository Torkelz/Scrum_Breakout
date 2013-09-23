#include "LevelGenerator.h"

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

LevelGenerator::~LevelGenerator(){}

void LevelGenerator::loadFile(string p_fileName)
{
	string line;
	char* lineChar;
	int offset = 0;
	fstream file;
	file.open(p_fileName);
	
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
				strcpy_s(lineChar, size, line.c_str());
				char* token;
				char* token_next;
				token = strtok_s(lineChar, ";", &token_next);
				int looplength = (int)(offset + size * 0.5f);

				for (int j = offset; j < looplength ; j++)
				{
					if (token != NULL)
					{
						int a = 0;
						a += atoi(token);
						m_loadedData.at(j) = a;
						token = strtok_s( NULL, ";", &token_next);
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
	for(int i = 0; i < 4; i++)
		m_blockLists.at(i).shrink_to_fit();
	
}

void LevelGenerator::createBlocks(FACE p_face)
{
	int size = sizeof(m_loadedData)/sizeof(int);
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
				addBlockToList(i, row, p_face);
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

vector<vector<ABlock*>>* LevelGenerator::getBlocks()
{
	return &m_blockLists;
}

void LevelGenerator::addBlockToList(int i, int row, FACE p_face)
{
	switch(p_face)
	{
		case FRONT:
			m_blockLists.at(0).push_back(new Block(&vec3(	m_posXOffsetFB+row*(g_bSizeX*2.1f),	
																		m_posYOffset+((-1)*(-5.0f+i*(g_bSizeY*2.2f))),
																		m_posZOffsetF),
																		&vec3(0.115f*i, 0.0f, 0.37f), "Block", 0));
			break;		
		case BACK:		
			m_blockLists.at(1).push_back(new Block(&vec3(		m_posXOffsetFB+row*(g_bSizeX*2.1f),
																		m_posYOffset+((-1)*(-5.0f+i*(g_bSizeY*2.2f))),
																		m_posZOffsetB),
																		&vec3(0.115f*i, 0.0f, 0.37f), "Block", 0));
			break;		
		case LEFT:		
			m_blockLists.at(2).push_back(new Block(&vec3(		m_posXOffsetL,
																		m_posYOffset+((-1)*(-5.0f+i*(g_bSizeY*2.2f))),
																		m_posZOffsetLR+row*(g_bSizeX*2.1f)),
																		&vec3(0.115f*i, 0.0f, 0.37f), "Block", 0));
			break;		
		case RIGHT:		
			m_blockLists.at(3).push_back(new Block(&vec3(	m_posXOffsetR,
																		m_posYOffset+((-1)*(-5.0f+i*(g_bSizeY*2.2f))),
																		m_posZOffsetLR+row*(g_bSizeX*2.1f)),
																		&vec3(0.115f*i, 0.0f, 0.37f), "Block", 0));
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
	m_posXOffsetL = -g_bSizeX;
	//Right X
	m_posXOffsetR = g_bSizeX * 2.0f * m_blockCountX + g_bSizeZ;
	//Left, Right Z
	m_posZOffsetLR = g_bSizeX;
	//Front Z
	m_posZOffsetF = 0.0f;
	//Back Z
	m_posZOffsetB = g_bSizeX * 2.0f * m_blockCountX + g_bSizeZ;
	//All Y
	m_posYOffset = 30.0f;
}	  
	  
	  
	  
	  
	  

