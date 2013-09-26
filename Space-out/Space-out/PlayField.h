#ifndef PLAYFIELD_H
#define PLAYFIELD_H
#include "ABlock.h"
#include <vector>

#include <gtc/matrix_transform.hpp>

inline int RoundDoneRight(float x) { if(x >= 0){ return (int)(x+0.5f); }else{ return (int)(x-0.5f); } }

using namespace std;

class PlayField
{
public:
	PlayField(vec3 p_positionOriginal, float p_angle, vec2 p_size);
	~PlayField();
	void init(vector<ABlock*> p_blockList, vec2 p_nrBlocks);
	void update();

	BlockVertex* getBufferData(); //Transfer to a array of blockvertex structs from blocklist
	int			 getListSize();
	mat4		getRotationMatrix();
private:
	vector<ABlock*> m_blockList;
	mat4			m_rotMatrixOriginal;
	vec2			m_size;
	vec3			m_positionOriginal;
	vector<BoundingVolume*>	m_borders;
	float			m_angle;
	vec3			m_planeVectorX;
	vec3			m_planeVectorY;
};

#endif