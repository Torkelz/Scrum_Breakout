#include "LevelGenerator.h"

LevelGenerator::LevelGenerator()
{

}

LevelGenerator::~LevelGenerator(){}

void LevelGenerator::loadFile(string p_fileName)
{
	string temp;
	char* temp1;
	char currentseg = NULL;
	int offset = 0;
	fstream m_file;
	m_file.open(p_fileName);
	int linecount = 1;
	
	if(m_file.is_open())
	{
		getline(m_file, temp);
		getline(m_file, temp);
		for(int i = 0; i < 4; i++)
		{
			while(temp.length()>1)
			{
				unsigned int size = temp.length() + 1;
				temp1 = new char[size];
				strcpy_s(temp1, size, temp.c_str());
				char* token;
				char* token_next;
				token = strtok_s(temp1, ";", &token_next);
				int looplength = (offset + size * 0.5f);

				for (int j = offset; j < looplength ; j++)
				{
					if (token != NULL)
					{
						int a = 0;
						a += atoi(token);
						m_loadedData[j] = a;
						if(a == 0)
						{
							int lol = 0;
						}
						token = strtok_s( NULL, ";", &token_next);
					}
				}
				if(offset < 180)
					offset += size/2;
				getline(m_file, temp);
			}
			createBlocks((FACE)i);
		}
	//	while (!m_file.eof())
	//	{
	//		getline(m_file, temp);
	//		int size = temp.length()+1;
	//		if(size == 2)
	//		{
	//			currentseg = temp[0];
	//			getline(m_file, temp);
	//			size = temp.length()+1;
	//		}
	//		temp1 = new char[temp.length() + 1];
	//		strcpy_s(temp1, size, temp.c_str());
	//		char* token;
	//		char* token_next;
	//		token = strtok_s(temp1, ";", &token_next);

	//		for (int i = offset; i < (offset + size/2) ; i++)
	//		{
	//			if (token != NULL)
	//			{
	//				int a = 0;
	//				a += atoi(token);
	//				m_loadedData[i] = a;
	//				token = strtok_s( NULL, ";", &token_next);
	//			}
	//		}
	//		if( offset < 180)
	//			offset += size/2;


	//		if (currentseg == 'f' && linecount == 10)
	//		{
	//			createBlocks(FRONT);
	//			linecount = 1;
	//		}
	//		else if(currentseg == 'b' && linecount == 10)
	//		{
	//			createBlocks(BACK);
	//			linecount = 1;
	//		}
	//		else if(currentseg == 'l' && linecount == 10)
	//		{
	//			createBlocks(LEFT);
	//			linecount = 1;
	//		}
	//		else if(currentseg == 'r' && linecount == 10)
	//		{
	//			createBlocks(RIGHT);
	//			linecount = 1;
	//		}
	//		else
	//		{
	//			linecount++;
	//		}
	//			
	//	}
	}
	m_blockLists.m_blocksFront.shrink_to_fit();
	m_blockLists.m_blocksBack.shrink_to_fit();
	m_blockLists.m_blocksLeft.shrink_to_fit();
	m_blockLists.m_blocksRight.shrink_to_fit();
}

void LevelGenerator::createBlocks(FACE p_face)
{
	int size = sizeof(m_loadedData)/sizeof(int);
	int x = (int)(size * 0.1f);
	int y = (int)(size * 0.05f);
	int offset = 0;
	int row = 0;
	for (int i = 0; i < y; i++)
	{
		for (int j = offset; j < (x + offset); j++)
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
		offset += x;
		row = 0;
	}
}

BlockLists* LevelGenerator::getBlocks()
{
	return &m_blockLists;
}

void LevelGenerator::addBlockToList(int i, int row, FACE p_face)
{
	switch(p_face)
	{
		case FRONT:
			m_blockLists.m_blocksFront.push_back(new Block(&Vector3((0+row*(g_bSizeX*2.1f)), 30+(-5.0f+i*(g_bSizeY*2.2f)), -15.0f), &Vector3(0.115f*i, 0.0f, 0.37f), "Block", 0));
			break;		
		case BACK:		
			m_blockLists.m_blocksBack.push_back(new Block(&Vector3((0+row*(g_bSizeX*2.1f)), 30+(-5.0f+i*(g_bSizeY*2.2f)), 35.0f), &Vector3(0.115f*i, 0.0f, 0.37f), "Block", 0));
			break;		
		case LEFT:		
			m_blockLists.m_blocksLeft.push_back(new Block(&Vector3(-15.0f, 30+(-5.0f+i*(g_bSizeY*2.2f)), (0+row*(g_bSizeZ*2.1f))), &Vector3(0.115f*i, 0.0f, 0.37f), "Block", 0));
			break;		
		case RIGHT:		
			m_blockLists.m_blocksRight.push_back(new Block(&Vector3(135.0f, 30+(-5.0f+i*(g_bSizeY*2.2f)), (0+row*(g_bSizeZ*2.1f))), &Vector3(0.115f*i, 0.0f, 0.37f), "Block", 0));
			break;
	}
};








