#ifndef AABB_H
#define AABB_H

#include "BoundingVolume.h"
#include "Sphere.h"
#include <vector>
#include <glm.hpp>
#include <gtx\norm.hpp>

struct Plane;
class Ray;
class Frustum;

using namespace glm;

class AABB : public BoundingVolume
{
public:
	static enum { OUTSIDE = 0, INTERSECT, INSIDE };

	AABB( vec3 p_top, vec3 p_bot, vec4 p_color );
	~AABB();

	void				calculateBounds();
	void				draw( mat4& p_world, mat4& p_view, mat4& p_proj );
	void				buildCubeIndices( int offset );
	void				updatePosition( mat4 p_scale, mat4 p_translate );
	void				initialize();
	vec3*				getMax();
	vec3*				getMin();
	int					boxVsBox( AABB* p_pBox );
	bool				boxVsSphere( Sphere* p_pSphere );
private:
	vec3				m_bottom;
	vec3				m_top;
	vec3				m_bounds[8];
	vec4				m_color;
	std::vector<int>	m_indices;
	float				m_distances[3];

	Sphere				m_sphere;

	vec3				m_center;
	vec3				m_halfDiagonal;
};

#endif