#include "BoundingVolume.h"

BoundingVolume::BoundingVolume()
{
	//m_index = G_NRofVOLUMES;
	//G_NRofVOLUMES++;
}


int BoundingVolume::getIndex()
{
	return m_index;
}

bool BoundingVolume::compair( BoundingVolume* p_volume )
{
	// Return true if it's the same volume, else return false.
	if(p_volume->getIndex() == m_index)
	{
		return true;
	}

	else
	{
		return false;
	}
}

int	BoundingVolume::getType()
{
	return m_type;
}

void BoundingVolume::updatePosition( mat4 p_scale, mat4 p_translate )
{
	m_prevPosition = m_position;

	vec4 v = vec4(m_position,0) * p_translate;

	 m_position.x = v.x;
	 m_position.y = v.y;
	 m_position.z = v.z;
}

void BoundingVolume::updatePosition( vec3 p_position )
{
	m_prevPosition = m_position;
	m_position = p_position;
}

vec3* BoundingVolume::getPosition()
{
	return &m_position;
}

vec3* BoundingVolume::getPrevPosition()
{
	return &m_prevPosition;
}

void BoundingVolume::draw(mat4& p_world, mat4& p_view, mat4& p_proj){}