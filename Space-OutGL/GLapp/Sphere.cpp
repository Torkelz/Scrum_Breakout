#include "Header/Sphere.h"

Sphere::Sphere()
{
	setRadius(0.0f);
	m_position = vec3(0,0,0);

	m_type = SPHERE;
}

Sphere::Sphere(float p_radius, vec3 p_pos) : BoundingVolume()
{
	setRadius(p_radius);
	m_position	= p_pos;

	m_type = SPHERE;
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

bool Sphere::sphereVsSphere (Sphere* p_sphere)
{
    vec3 CDiff;
	CDiff = *p_sphere->getPosition() - m_position;
	
	float c = length2(CDiff);
	float rSum = p_sphere->getRadius() + m_radius;
    float rSumSqr = rSum*rSum;

    return c <= rSumSqr;
}
