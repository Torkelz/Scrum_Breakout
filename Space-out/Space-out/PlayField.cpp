#include "PlayField.h"


PlayField::PlayField(vec3 p_positionOriginal, float p_angle, vec2 p_size)
{
	m_angle = p_angle;
	m_planeVectorX = vec3(1.0f, 0.0f, 0.0f);
	m_planeVectorY = vec3(0.0f, -1.0f, 0.0f);

	m_rotMatrixOriginal = mat4(1.0f);
	m_rotMatrixOriginal = rotate(m_rotMatrixOriginal, m_angle, vec3(0,1,0));
	mat4 m_trans = mat4();
	m_trans = translate(m_trans, -p_positionOriginal);

	//Move originposition to right coordinates
	vec4 tempOrigin = vec4(p_positionOriginal,0.f);
	tempOrigin = m_trans * tempOrigin;
	tempOrigin = m_rotMatrixOriginal * tempOrigin;
	m_trans = translate(m_trans, p_positionOriginal);
	tempOrigin = m_trans * tempOrigin;

	m_positionOriginal = vec3(RoundDoneRight(tempOrigin.x) , RoundDoneRight(tempOrigin.y), RoundDoneRight(tempOrigin.z));

	//Rotate the direction of the plane
	vec4 tempX = vec4(m_planeVectorX,0.f);
	vec4 tempY = vec4(m_planeVectorY,0.f);
	tempX = m_rotMatrixOriginal * tempX;
	tempY = m_rotMatrixOriginal * tempY;
	
	m_planeVectorX = vec3(RoundDoneRight(tempX.x),RoundDoneRight(tempX.y),RoundDoneRight(tempX.z));
	m_planeVectorY = vec3(RoundDoneRight(tempY.x),RoundDoneRight(tempY.y),RoundDoneRight(tempY.z));

	m_size = p_size;
}

PlayField::~PlayField()
{

}

void PlayField::init(vector<ABlock*> p_blockList, vec2 p_nrBlocks)
{
	m_blockList = p_blockList;
	vec3 orto; // planevecX * planeVecY pekar mot mot center
	orto = cross( m_planeVectorX, m_planeVectorY );
	float offsetX = 0.f, offsetY = 0.f, offsetZ = 0.f, sizeX = 1.f, sizeZ = 1.f;
	
	//Awesome linear Algebra
	//Left AABB
	m_borders.push_back(new AABB(m_positionOriginal + (m_planeVectorX * offsetX) -(m_planeVectorY * offsetY) + (orto * offsetZ), 
								m_positionOriginal + (m_planeVectorX * (offsetX - sizeX)) -(m_planeVectorY * (offsetY + m_size.y)) + (orto * (offsetZ - sizeZ)), 
								vec4(1.0f, 1.0f, 1.0f, 1.0f)));
	//Right AABB
	vec3 tempOriginPos = m_positionOriginal + (m_planeVectorX * m_size.x);

	m_borders.push_back(new AABB(tempOriginPos - (m_planeVectorX * offsetX) -(m_planeVectorY * offsetY) + (orto * offsetZ), 
								tempOriginPos - (m_planeVectorX * (offsetX - sizeX)) -(m_planeVectorY * (offsetY + m_size.y)) + (orto * (offsetZ - sizeZ)), 
								vec4(1.0f, 1.0f, 1.0f, 1.0f)));
	//Top AABB
	m_borders.push_back(new AABB ( m_positionOriginal - (m_planeVectorX * (offsetX - m_size.x)) -(m_planeVectorY * (offsetY + sizeX)) + (orto * (offsetZ - sizeZ)), 
								m_positionOriginal + (m_planeVectorX * offsetX) -(m_planeVectorY * offsetY) + (orto * offsetZ),
								vec4(1.0f, 1.0f, 1.0f, 1.0f)));
	//Bottom AABB
	tempOriginPos = m_positionOriginal - (m_planeVectorY * m_size.y);
	m_borders.push_back(new AABB ( tempOriginPos - (m_planeVectorX * (offsetX - m_size.x)) -(m_planeVectorY * (offsetY + sizeX)) + (orto * (offsetZ - sizeZ)), 
								tempOriginPos - (m_planeVectorX * offsetX) -(m_planeVectorY * offsetY) + (orto * offsetZ),
								vec4(1.0f, 1.0f, 1.0f, 1.0f)));
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
		/*pos += m_planeVectorX *  g_bvSize.x +  temp.x * (m_planeVectorX *  g_bvSize.x * 2.0f);
		pos += m_planeVectorY *  g_bvSize.y +  temp.y * (m_planeVectorY *  g_bvSize.y * 2.0f);*/
		m_blockList.at(i)->setPos(pos);
	}
}

void PlayField::update()
{

}

BlockVertex* PlayField::getBufferData()
{
  BlockVertex* temp = NULL;
  unsigned int size = m_blockList.size();
  temp = new BlockVertex[size];
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

