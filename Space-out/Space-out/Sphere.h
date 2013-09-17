#ifndef SPHERE_H
#define SPHERE_H

#include "BoundingVolume.h"

class Sphere : public BoundingVolume
{
public:
	Sphere();
	Sphere(float p_radius, vec3 p_pos);
	~Sphere();

	float	getRadius();
	float	getSqrRadius();
	void	setRadius(float p_radius);
	bool	sphereVsSphere(Sphere* p_sphere);

private:
	float	m_radius;
	float	m_sqrRadius;
};

#endif SPHERE_H