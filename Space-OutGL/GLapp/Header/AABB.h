#ifndef AABB_H
#define AABB_H

#include "BoundingVolume.h"
#include "Sphere.h"
#include <vector>

#include <glm/gtx/rotate_vector.hpp>
#define PI 3.141592
class AABB : public BoundingVolume
{
public:
	enum { OUTSIDE = 0, INTERSECT, INSIDE };
	enum { TOP = 0, BOTTOM, LEFT, RIGHT, CORNER };

	// ## MUST BE BUILT IN MODEL SPACE! ##
	AABB( vec3 p_top, vec3 p_bot, vec4 p_color );
	~AABB();

	void				calculateBounds();
	void				updatePosition( mat4 p_scale,mat4 p_rotation,mat4 p_translate );
	void				initialize();
	vec3*				getMax();
	vec3*				getMin();
	int					boxVsBox( AABB* p_pBox );
	bool				boxVsSphere( Sphere* p_pSphere );
	bool				collide( BoundingVolume* p_pVolume );
	vec3				findNewDirection(vec3 p_sphereCenter, vec3 p_speed);
	int					findPlane(vec3 p_sphereCenter);
	// ## NOT FOR BORDERS, EVER ##
	void				calculateAngle();
	// ## FOR BORDERS AND BLOCKS, MAYBE POWERUPS ##
	void				calculateAngle( bool p_x, bool p_border);

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
	float				m_v, m_w, m_v2, m_w2, cornerAngles[8];
};

#endif