#ifndef LEVELGENERATOR_H
#define LEVELGENERATOR_H

#include <string>
#include <fstream>
#include <sstream>


using namespace std;

class LevelGenerator
{
public:
	LevelGenerator();
	~LevelGenerator();

	void loadFile(string p_fileName);


private:
	ifstream		m_file;
	stringstream	m_stream;
	int*			m_loadedData;
};

#endif 
