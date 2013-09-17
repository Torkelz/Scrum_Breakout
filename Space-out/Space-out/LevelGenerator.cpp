#include "LevelGenerator.h"

LevelGenerator::LevelGenerator()
{

}

LevelGenerator::~LevelGenerator(){}

void LevelGenerator::loadFile(string p_fileName)
{
	string temp;
	char* temp1;
	m_file.open(p_fileName);

	if(m_file.is_open())
	{
		while (!m_file.eof())
		{
			getline(m_file, temp);
			m_stream << temp;
			int size = temp.length() +1;
			temp1 = new char[temp.length() +1];
			strcpy_s(temp1, size, temp.c_str());
			char* token;
			token = strtok(temp1,";");
		}
	}

}





