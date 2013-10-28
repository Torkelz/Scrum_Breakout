#ifndef CAMERA_H
#define CAMERA_H
#define GLM_SWIZZLE
#define PI 3.141592f

#include <glm/glm.hpp>
#include <glm/gtx/spline.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform2.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtc/swizzle.hpp>
#include <vector>

using namespace glm;
class Camera
{
public:
	Camera();
	~Camera();

	void				init(vec3 initPos);

	void				createProjectionMatrix( float p_FOV, float p_aspect,
												float p_nearPlane, float p_farPlane );
	void				updateCameraPos(float p_dt);
	void				updateViewMatrix();
	void				setViewMatrix();
	void				setY( float p_f );
	void				setPosition(float p_x, float p_y, float p_z);
	void				walk( float p_f );
	void				strafe( float p_f );
	void				updateYaw( float p_d );
	void				updatePitch( float p_d );
	bool 				getRunOnce();
	void 				setRunOnce(bool p);

	// Spline stuffies
	
	void				buildPath(vec3 p_start, vec3 p_stop, vec3 p_originWorld, int p_nrPoints);
	bool				isCinematic();
	void				startCinematic();
	bool				timeToChange();

	void				setYaw(int p_activePlayField);
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
	float			m_yawStart;
	float			m_yawNext;
	bool			m_runOnce;

	// Spline thingy
	bool				m_isCinematic;
	float				m_lifeTime;
	float				m_angleToNextPF;

	std::vector<vec3>	m_cinematicPos;

	void				flyCinematic(float p_lifeTime);
	void				interpolateYaw(float p_lifeTime);
	void				resetCinematic();


	// End spline thingys

	float			cotan(float p_i);
};

#endif /* CAMERA_H */