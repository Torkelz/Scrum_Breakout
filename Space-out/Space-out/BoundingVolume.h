#ifndef BOUNDINGVOLUME_H
#define BOUNDINGVOLUME_H

#include <glm.hpp>

using namespace glm;

class BoundingVolume
{
public:
	BoundingVolume();

	int				getIndex();
	bool			compair( BoundingVolume* p_volume );

	virtual void	updatePosition( mat4 p_scale, mat4 p_translation );
	virtual void	updatePosition( vec3 p_position );
	vec3*			getPosition();
	vec3*			getPrevPosition();
	int				getType();
	virtual void	draw(mat4& p_world, mat4& p_view, mat4& p_proj);
protected:
	vec3			m_position;
	vec3			m_prevPosition;
	int				m_index;
	int				m_type;

	enum TYPE {
		AABBOX,
		FRUSTUM
	};
};

#endif