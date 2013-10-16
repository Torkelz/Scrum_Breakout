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
	void init(vector<ABlock*> p_blockList, vec2 p_nrBlocks, bool p_x);
	void transBorders(bool p_x);
	void update();

	BlockVertex*	getBufferData(); //Transfer to a array of blockvertex structs from blocklist
	int				getListSize();
	mat4			getRotationMatrix();
	ABlock*			getBlock(unsigned int p_id);
	void			deleteBlock(unsigned int p_id);

	vec3			calculateCameraCenterPos();

	bool			getUpdateBuffer();
	void			setUpdateBuffer(bool p_bool);
	BoundingVolume* getCollisionBorder(unsigned int p_id);
	unsigned int	getNrBorders();
	vec3			getOriginalPosition();
	vec2			getScreenPosition(mat4 viewproj);
	vec3			getRightDir();
	vec3			getDownDir();
	vec2			getSize();
private:
	vector<ABlock*> m_blockList;
	mat4			m_rotMatrixOriginal;
	vec2			m_size;
	vec3			m_borderSize;
	vec3			m_positionOriginal;
	vector<BoundingVolume*>	m_borders;
	float			m_angle;
	vec3			m_planeVectorX;
	vec3			m_planeVectorY;
	bool			m_updateBuffer;
	vec3			m_borderOffset;

	// spline stuffies
	
	vec3 m_orto;		// planevecX * planeVecY pekar mot mot center
};

#endif