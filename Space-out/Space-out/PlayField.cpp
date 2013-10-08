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
	//vec3 orto;		// planevecX * planeVecY pekar mot mot center
	//orto			= cross( m_planeVectorX, m_planeVectorY );
	//float sizeZ = 150.f;
	m_borderSize = vec3((m_size.x/2) ,(m_size.y/2), m_size.x/2 );
	vec3 tBorders = m_borderSize;
	//m_borderSize += m_borderOffset;
	////vec3 borderSize = vec3(20 ,20,20);
	//mat4 trans;
	////borderSize = vec3(m_rotMatrixOriginal * vec4(borderSize,0.f));

	//vec3 center = m_positionOriginal - m_planeVectorX * abs(dot(m_planeVectorX, borderSize));
	//center		+= m_planeVectorY * abs(dot(m_planeVectorY, borderSize));
	tBorders.y += m_borderOffset.y;
	//Left AABB
	m_borders.push_back(new AABB(tBorders,-tBorders,vec4(1.0f, 1.0f, 1.0f, 1.0f)));
	/*trans = translate(mat4(1.0f), vec3(50,0,0));
	m_borders.back()->updatePosition(mat4(1.0f),trans);
	((AABB*)m_borders.back())->calculateAngle(p_x);
	*/
	//Right AABB
	/*center += m_planeVectorX * (m_size.x + (abs(dot(m_planeVectorX, borderSize))*2));*/
	m_borders.push_back(new AABB(tBorders,-tBorders, vec4(1.0f, 1.0f, 1.0f, 1.0f)));
	/*trans = translate(mat4(1.0f), center);
	m_borders.back()->updatePosition(mat4(1.0f),trans);
	((AABB*)m_borders.back())->calculateAngle(p_x);*/
	tBorders.y -= m_borderOffset.y;
	//Top AABB	
	/*center = m_positionOriginal + m_planeVectorX * abs(dot(m_planeVectorX, borderSize));
	center		+= -m_planeVectorY * abs(dot(m_planeVectorY, borderSize));
*/
	tBorders.x += m_borderOffset.x;
	tBorders.z += m_borderOffset.z;
	m_borders.push_back(new AABB (tBorders,-tBorders,	vec4(1.0f, 1.0f, 1.0f, 1.0f)));
	/*trans = translate(mat4(1.0f), center);
	m_borders.back()->updatePosition(mat4(1.0f),trans);
	((AABB*)m_borders.back())->calculateAngle(p_x);*/

	//Bottom AABB
	/*center += m_planeVectorY * (m_size.y + (abs(dot(m_planeVectorY, borderSize))*2));*/
	m_borders.push_back(new AABB(tBorders,-tBorders, vec4(1.0f, 1.0f, 1.0f, 1.0f)));
	tBorders.x -= m_borderOffset.x;
	tBorders.z -= m_borderOffset.z;
	/*trans = translate(mat4(1.0f), center);
	m_borders.back()->updatePosition(mat4(1.0f),trans);
	((AABB*)m_borders.back())->calculateAngle(p_x);*/
	
	//Block Placement STUFF!
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
	vec3 orto;		// planevecX * planeVecY pekar mot mot center
	orto			= cross( m_planeVectorX, m_planeVectorY );
	//vec3 borderSize = vec3(20 ,20,20);
	mat4 trans;
	//borderSize = vec3(m_rotMatrixOriginal * vec4(borderSize,0.f));

	vec3 center = m_positionOriginal - m_planeVectorX * abs(dot(m_planeVectorX, m_borderSize));
	center		+= m_planeVectorY * abs(dot(m_planeVectorY, m_borderSize));
	//Left
	((AABB*)m_borders.at(0))->calculateAngle(p_x, true);
	trans = translate(mat4(1.0f), center);
	m_borders[0]->updatePosition(mat4(1.0f),mat4(1.0f),trans);
	
	center += m_planeVectorX * (m_size.x + (abs(dot(m_planeVectorX, m_borderSize))*2));
	trans = translate(mat4(1.0f), center);
	//Right
	((AABB*)m_borders.at(1))->calculateAngle(p_x, true);
	m_borders[1]->updatePosition(mat4(1.0f),mat4(1.0f),trans);

	center = m_positionOriginal + m_planeVectorX * abs(dot(m_planeVectorX, m_borderSize));
	center		+= -m_planeVectorY * abs(dot(m_planeVectorY, m_borderSize));
	//Top
	((AABB*)m_borders.at(2))->calculateAngle(p_x, true);
	trans = translate(mat4(1.0f), center);
	m_borders[2]->updatePosition(mat4(1.0f),mat4(1.0f),trans);
	//Bottom
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