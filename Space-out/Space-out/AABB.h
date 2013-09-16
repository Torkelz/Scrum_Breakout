#ifndef AABB_H
#define AABB_H

#include "BoundingVolume.h"
#include "Sphere.h"
#include <vector>
#include <glm.hpp>

struct Plane;
class Ray;
class Frustum;

using namespace glm;

struct BoxPlane
{
	// p = ax + by + cz or p = a dot r where p is a point in the BoxPlane.
	// a is a vector pointing at a point in the BoxPlane, r is pointing at 
	// another and r - a is perpendicular to the BoxPlane normal.
	// Which means the BoxPlane could be described as n dot ( r - a ) = 0.

	vec3		normal;
	vec3		a;
	float			distance; // lenght of vector a.
	
	BoxPlane(){}
	BoxPlane( vec3 v1, vec3 v2, vec3 v3){ setUpBoxPlane(v1, v2, v3); }
	void setUpBoxPlane( vec3 v1, vec3 v2, vec3 v3 )
	{
		vec3 v, u;

		v = v1 - v2;
		u = v3 - v2;

		normal = cross(u, v);
		normal = normalize(normal);
		a = v2;

		distance = - dot( normal, a );
	}

	float calculateDistance( vec3 v )
	{
		return ( distance + dot( normal, v ) );
	}
};

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
	vec3*		getMax();
	vec3*		getMin();
	int					boxVsBox( AABB* p_box );
	int					boxVsFrustum( Frustum* p_frustum );
	bool				rayVsBox( Ray* p_ray );
	int					boxVsPlane( Plane* p_plane );
	int					collide( BoundingVolume* p_volume );
	void				rayHit();
	void				frustumHit();

private:
	enum {
		TOP = 0,
		BOTTOM,
		LEFT,
		RIGHT,
		NEARP,
		FARP
	};

	vec3			m_bottom;
	vec3			m_top;
	vec3			m_bounds[8];
	vec4			m_color;
	std::vector<int>	m_indices;
	BoxPlane			m_planes[6];
	float				m_distances[3];

	// Needed for frustum collison-tests.
	vec3			m_center;
	vec3			m_halfDiagonal;
	Sphere			m_sphere;
};

#endif