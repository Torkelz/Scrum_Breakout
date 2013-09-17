#include "Sphere.h"

#include <gtx\norm.hpp>

Sphere::Sphere()
{
	m_radius = 0.0f;
	m_position = vec3(0,0,0);
}

Sphere::Sphere(float p_radius, vec3 p_pos) : BoundingVolume()
{
	m_radius	= p_radius;
	m_position	= p_pos;
}

Sphere::~Sphere(){}

float Sphere::getRadius()
{
	return m_radius;
}

float Sphere::getSqrRadius()
{
	return m_sqrRadius;
}

void Sphere::setRadius(float p_radius)
{
	m_radius = p_radius;
	m_sqrRadius = m_radius * m_radius;
}

//bool Sphere::sphereVsSphere(Sphere* p_sphere)
//{
//	// Get the distance between the two spheres.
//	D3DXVECTOR3 distance;
//	D3DXVec3Subtract(&distance, &m_position, p_sphere->getPosition());
//	float d = D3DXVec3LengthSq(&distance);
//
//	// Compair distance with the spheres radius squared.
//	if ( d > ( m_sqrRadius * p_sphere->getSqrRadius() ) )
//	{
//		// If bigger.
//		return false;
//	}
//
//	return true;
//}

bool Sphere::sphereVsSphere (Sphere* p_sphere)
{
    vec3 CDiff;
	//D3DXVec3Subtract(&CDiff, p_sphere->getPosition(), &m_position);
	CDiff = *p_sphere->getPosition() - m_position;

	//float c = D3DXVec3LengthSq(&CDiff);
	float c = length2(CDiff);
	float rSum = p_sphere->getRadius() + m_radius;
    float rSumSqr = rSum*rSum;

    return c <= rSumSqr;
}