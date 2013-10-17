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
	mat4 rot = rotate(mat4(1.0f), m_angle, vec3(0,1,0));
	tempX		= rot * tempX;
	tempY		= rot * tempY;
	
	m_planeVectorX = vec3(RoundDoneRight(tempX.x),RoundDoneRight(tempX.y),RoundDoneRight(tempX.z));
	m_planeVectorY = vec3(RoundDoneRight(tempY.x),RoundDoneRight(tempY.y),RoundDoneRight(tempY.z));

	m_size			= p_size;
	m_updateBuffer	= false;

	m_borderOffset = vec3(20,20,20);
}

PlayField::~PlayField()
{

}

void PlayField::init(vector<ABlock*> p_blockList, vec2 p_nrBlocks, bool p_x)
{
	m_blockList		= p_blockList;
	m_borderSize = vec3((m_size.x/2) ,(m_size.y/2), m_size.x/2 );
	vec3 tBorders = m_borderSize;
	tBorders.y += m_borderOffset.y;

	//Left AABB
	m_borders.push_back(new AABB(tBorders,-tBorders,vec4(1.0f, 1.0f, 1.0f, 1.0f)));
	//Right AABB
	m_borders.push_back(new AABB(tBorders,-tBorders, vec4(1.0f, 1.0f, 1.0f, 1.0f)));
	tBorders.y -= m_borderOffset.y;

	//Top AABB
	tBorders.x += m_borderOffset.x;
	tBorders.z += m_borderOffset.z;
	m_borders.push_back(new AABB (tBorders,-tBorders,	vec4(1.0f, 1.0f, 1.0f, 1.0f)));

	//Bottom AABB
	m_borders.push_back(new AABB(tBorders,-tBorders, vec4(1.0f, 1.0f, 1.0f, 1.0f)));
	tBorders.x -= m_borderOffset.x;
	tBorders.z -= m_borderOffset.z;
	
	//Block Placement
	unsigned int l = m_blockList.size();
	for(unsigned int i = 0; i < l; i++)
	{
		vec2 temp = m_blockList.at(i)->getBlockID();
		vec3 pos = m_positionOriginal;
		vec3 dirX = m_planeVectorX * m_size.x/ p_nrBlocks.x;
		vec3 dirY = m_planeVectorY * (m_size.y/2)/ p_nrBlocks.y;

		pos += dirX * 0.5f + dirX * temp.x;
		pos += dirY * 0.5f + dirY * temp.y;  
		m_blockList.at(i)->setPos(pos, &m_rotMatrixOriginal);
		((AABB*)m_blockList.at(i)->getBoundingVolume())->calculateAngle(p_x, false);
	}
}

void PlayField::transBorders(bool p_x)
{
	mat4 trans;
	
	//## Left ##
	vec3 center = m_positionOriginal - m_planeVectorX * abs(dot(m_planeVectorX, m_borderSize));
	center		+= m_planeVectorY * abs(dot(m_planeVectorY, m_borderSize));
	((AABB*)m_borders.at(0))->calculateAngle(p_x, true);
	trans = translate(mat4(1.0f), center);
	m_borders[0]->updatePosition(mat4(1.0f),mat4(1.0f),trans);
	
	//## Right ##
	center += m_planeVectorX * (m_size.x + (abs(dot(m_planeVectorX, m_borderSize))*2));
	trans = translate(mat4(1.0f), center);
	((AABB*)m_borders.at(1))->calculateAngle(p_x, true);
	m_borders[1]->updatePosition(mat4(1.0f),mat4(1.0f),trans);
	
	//## Top ##
	center = m_positionOriginal + m_planeVectorX * abs(dot(m_planeVectorX, m_borderSize));
	center		+= -m_planeVectorY * abs(dot(m_planeVectorY, m_borderSize));
	((AABB*)m_borders.at(2))->calculateAngle(p_x, true);
	trans = translate(mat4(1.0f), center);
	m_borders[2]->updatePosition(mat4(1.0f),mat4(1.0f),trans);

	//## Bottom ##
	((AABB*)m_borders.at(3))->calculateAngle(p_x, true);
	center += m_planeVectorY * (m_size.y + (abs(dot(m_planeVectorY, m_borderSize))*2));
	trans = translate(mat4(1.0f), center);
	m_borders[3]->updatePosition(mat4(1.0f),mat4(1.0f),trans);
}

void PlayField::update()
{

}

BlockVertex* PlayField::getBufferData()
{
  BlockVertex* temp = NULL;
  unsigned int size = m_blockList.size();
  temp				= new BlockVertex[size];
  for(unsigned int i = 0; i < size; i++)
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

//The offset in y axis is done because the camera is shifted down ( pitch )
vec3 PlayField::calculateCameraCenterPos()
{
	m_orto			= cross( m_planeVectorX, m_planeVectorY );

	vec3 pos;

	pos = m_positionOriginal + (m_planeVectorX * m_size.x * 0.5f) + (m_planeVectorY * m_size.y * 0.5f);
	pos = pos + m_orto * 200.0f;

	pos += vec3(0.0f, -35.0f, 0.0f);

	return pos;
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

vec3 PlayField::getOriginalPosition()
{
	return m_positionOriginal;
}

vec2 PlayField::getScreenPosition(mat4 viewproj)
{
	vec4 rr = viewproj * vec4(m_positionOriginal,1.f);
	rr /= rr.w;

	return vec2(rr);
}

vec3 PlayField::getRightDir()
{
	return m_planeVectorX;
}
vec3 PlayField::getDownDir()
{
	return m_planeVectorY;
}

vec2 PlayField::getSize()
{
	return m_size;
}