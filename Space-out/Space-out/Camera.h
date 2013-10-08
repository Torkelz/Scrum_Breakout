#ifndef CAMERA_H
#define CAMERA_H
#define GLM_SWIZZLE
#define PI 3.141592f

#include <glm.hpp>
#include <gtc\matrix_transform.hpp>
#include <gtx\transform.hpp>
#include <gtx\rotate_vector.hpp>

using namespace glm;
class Camera
{
public:
	Camera(vec3 initPos);
	~Camera();

	void				createProjectionMatrix( float p_FOV, float p_aspect,
												float p_nearPlane, float p_farPlane );
	void				updateCameraPos();
	void				updateViewMatrix();
	void				setViewMatrix(vec3 p_pos);
	void				setY( float p_f );
	void				setPosition(float p_x, float p_y, float p_z);
	void				walk( float p_f );
	void				strafe( float p_f );
	void				updateYaw( float p_d );
	void				updatePitch( float p_d );
	vec3				getPosition();
	vec3				getLookAt();
	mat4				getProjectionMatrix();
	mat4				getViewMatrix();
	float				getFarPlane();
	
private:
	vec3			m_cameraPos;
	vec3			m_up;
	vec3			m_lookAtPos;
	vec3			m_velocity;
	vec3			m_lookAt;
	vec3			m_right;
					 
	mat4			m_view;
	mat4			m_projection;
					 
	float			m_FOV;
	float			m_aspect;
	float			m_nearPlane;
	float			m_farPlane;
					 
	float			m_pitch;
	float			m_MAXpitch;
	float			m_MINpitch;
	float			m_yaw;

	float			cotan(float p_i);
};

#endif CAMERA_H