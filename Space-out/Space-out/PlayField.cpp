#include "PlayField.h"


PlayField::PlayField(vec3 p_positionOriginal, float p_angle, vec2 p_size)
{
	m_angle			= p_angle;
	m_planeVectorX	= vec3(-1.0f, 0.0f, 0.0f);
	m_planeVectorY	= vec3(0.0f, -1.0f, 0.0f);

	m_rotMatrixOriginal = mat4(1.0f);
	m_rotMatrixOriginal = rotate(m_rotMatrixOriginal, m_angle, vec3(0,1,0));
	mat4 m_trans		= mat4();
	m_trans				= translate(m_trans, -p_positionOriginal);

	//Move originposition to right coordinates
	vec4 tempOrigin = vec4(p_positionOriginal,0.f);
	tempOrigin		= m_trans * tempOrigin;
	tempOrigin		= m_rotMatrixOriginal * tempOrigin;
	m_trans			= translate(m_trans, p_positionOriginal);
	tempOrigin		= m_trans * tempOrigin;

	m_positionOriginal = vec3(RoundDoneRight(tempOrigin.x) , RoundDoneRight(tempOrigin.y), RoundDoneRight(tempOrigin.z));

	//Rotate the direction of the plane
	vec4 tempX	= vec4(m_planeVectorX,0.f);
	vec4 tempY	= vec4(m_planeVectorY,0.f);
	tempX		= m_rotMatrixOriginal * tempX;
	tempY		= m_rotMatrixOriginal * tempY;
	
	m_planeVectorX = vec3(RoundDoneRight(tempX.x),RoundDoneRight(tempX.y),RoundDoneRight(tempX.z));
	m_planeVectorY = vec3(RoundDoneRight(tempY.x),RoundDoneRight(tempY.y),RoundDoneRight(tempY.z));

	m_size			= p_size;
	m_updateBuffer	= false;
}

PlayField::~PlayField()
{

}

void PlayField::init(vector<ABlock*> p_blockList, vec2 p_nrBlocks)
{
	m_blockList		= p_blockList;
	vec3 orto;		// planevecX * planeVecY pekar mot mot center
	orto			= cross( m_planeVectorX, m_planeVectorY );
	float offsetX	= 0.f, offsetY = 150.f, offsetZ = -1.f, sizeX = 150.f, sizeZ = 150.f;
	mat4 trans;

	//Left AABB
	vec3 top	= m_positionOriginal - (m_planeVectorX * (offsetX + sizeX)) -(m_planeVectorY * offsetY) + (orto * ((offsetZ + sizeZ)/2));
	vec3 bottom	= m_positionOriginal - (m_planeVectorX * offsetX) +(m_planeVectorY * (offsetY + m_size.y)) - (orto * ((offsetZ + sizeZ)/2));
	
	m_borders.push_back(new AABB(top,bottom,vec4(1.0f, 1.0f, 1.0f, 1.0f)));
	
	//Right AABB
	trans			= translate(mat4(1.0f),vec3(-(m_size.x + sizeX),0.f,0.f));
	vec4 temptop	= vec4(top, 1.0f);
	vec4 tempbottom = vec4(bottom, 1.0f);
	temptop			= trans * temptop;
	tempbottom		= trans * tempbottom;
	top				= vec3(temptop);
	bottom			= vec3(tempbottom);

	m_borders.push_back(new AABB(top, bottom, vec4(1.0f, 1.0f, 1.0f, 1.0f)));

	//Top AABB
	top		= m_positionOriginal - (m_planeVectorX * (offsetY)) -(m_planeVectorY * (offsetX + sizeX)) + (orto * ((offsetZ + sizeZ)/2));
	bottom	= m_positionOriginal + (m_planeVectorX * (m_size.x + offsetY)) -(m_planeVectorY) - (orto * ((offsetZ + sizeZ)/2));

	m_borders.push_back(new AABB ( top,	bottom,	vec4(1.0f, 1.0f, 1.0f, 1.0f)));
	
	//Bottom AABB
	trans		= translate(mat4(1.0f),vec3(0.f,-(m_size.y+sizeZ),0.f));
	temptop		= vec4(top, 1.0f);
	tempbottom	= vec4(bottom, 1.0f);
	temptop		= trans * temptop;
	tempbottom	= trans * tempbottom;
	top			= vec3(temptop);
	bottom		= vec3(tempbottom);

	m_borders.push_back(new AABB ( top, bottom,	vec4(1.0f, 1.0f, 1.0f, 1.0f)));
	
	//Block Placement STUFF!
	unsigned int l = m_blockList.size();
	for(int i = 0; i < l; i++)
	{
		vec2 temp = m_blockList.at(i)->getBlockID();
		vec3 pos = m_positionOriginal;
		vec3 dirX = m_planeVectorX * m_size.x/ p_nrBlocks.x;
		vec3 dirY = m_planeVectorY * (m_size.y/2)/ p_nrBlocks.y;

		pos += dirX * 0.5f + dirX * temp.x;
		pos += dirY * 0.5f + dirY * temp.y;  
		m_blockList.at(i)->setPos(pos, &m_rotMatrixOriginal);
	}
}

void PlayField::update()
{

}

BlockVertex* PlayField::getBufferData()
{
  BlockVertex* temp = NULL;
  unsigned int size = m_blockList.size();
  temp				= new BlockVertex[size];
  for(int i = 0; i < size; i++)
  {
	  temp[i] = m_blockList.at(i)->getBlockVertex();
  }
  return temp;
}

int PlayField::getListSize()
{
	return m_blockList.size();
}




mat4 PlayField::getRotationMatrix()
{
	return m_rotMatrixOriginal;
}


ABlock* PlayField::getBlock(unsigned int p_id)
{
	return m_blockList.at(p_id);
}

void PlayField::deleteBlock(unsigned int p_id)
{
	m_blockList.erase(m_blockList.begin() + p_id);
	setUpdateBuffer(true);
}


bool PlayField::getUpdateBuffer()
{
	return m_updateBuffer;
}

void PlayField::setUpdateBuffer(bool p_bool)
{
	m_updateBuffer = p_bool;
}

BoundingVolume* PlayField::getCollisionBorder(unsigned int p_id)
{
	return m_borders.at(p_id);
}

unsigned int PlayField::getNrBorders()
{
	return m_borders.size();
}