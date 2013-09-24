#include "PlayField.h"


PlayField::PlayField(vec3 p_positionOriginal, float p_angle, vec2 p_size)
{
	m_angle = p_angle;
	m_planeVector = vec3(1,-1,0);

	m_rotMatrixOriginal = mat4(1.0f);
	m_rotMatrixOriginal = rotate(m_rotMatrixOriginal, m_angle, vec3(0,1,0));
	mat4 m_trans = mat4();
	m_trans = translate(m_trans, -p_positionOriginal);

	//Move originposition to right coordinates
	vec4 temp = vec4(p_positionOriginal,0.f);
	temp = m_trans * temp;
	temp = m_rotMatrixOriginal * temp;
	m_trans = translate(m_trans, p_positionOriginal);
	temp = m_trans * temp;

	//Rotate the direction of the plane
	vec4 temp1 = vec4(m_planeVector,0.f);
	temp1 = m_rotMatrixOriginal * temp1;
	//temp1 = normalize(temp1);

	m_planeVector = vec3(RoundDoneRight(temp1.x),RoundDoneRight(temp1.y),RoundDoneRight(temp1.z));

	m_positionOriginal = vec3(RoundDoneRight(temp.x) , RoundDoneRight(temp.y), RoundDoneRight(temp.z));
	
	m_size = p_size;
}

PlayField::~PlayField()
{

}

void PlayField::init(vector<ABlock*> p_blockList)
{
	m_blockList = p_blockList;


}

void PlayField::update()
{

}

BlockVertex* PlayField::getBufferData()
{
  BlockVertex* lol = NULL;
  return lol; //For Shutting up Intellisense
}







