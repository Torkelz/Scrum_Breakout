#include "Camera.h"


Camera::Camera(vec3 initPos)
{
	m_cameraPos	= initPos;
	m_velocity	= vec3( 0.0f, 0.0f, 0.0f );

	m_right		= vec3( 1.0f, 0.0f, 0.0f );
	m_up			= vec3( 0.0f, 1.0f, 0.0f );
	m_lookAt		= vec3( 0.0f, 0.0f, 1.0f );

	m_pitch		= 0.0f;
	m_yaw		= 0.0f;
	m_lookAtPos	= m_cameraPos + m_lookAt;

	m_view = mat4(1.0f);
	m_projection = mat4(1.0f);
	
	m_MAXpitch	= PI/2.0f;
	m_MINpitch	= -PI/2.0f;
}

Camera::~Camera(){}

void Camera::createProjectionMatrix( float p_FOV, float p_aspect,
									float p_nearPlane, float p_farPlane )
{
	m_FOV		= p_FOV;
	m_aspect		= p_aspect;
	m_nearPlane	= p_nearPlane;
	m_farPlane	= p_farPlane;

	float yScale	= cotan( m_FOV / 2.0f );
	float xScale	= yScale / m_aspect;
	float planeDiff	= m_farPlane - m_nearPlane;

	// Lens.
	m_projection[0].x = xScale;
	m_projection[1].y = yScale;
	m_projection[2].z = m_farPlane / planeDiff;
	m_projection[3].z = ( m_nearPlane * m_farPlane ) / ( m_nearPlane - m_farPlane );
	m_projection[2].w = 1.0f;
	m_projection[3].w = 0.0f;

}

void Camera::updateViewMatrix()
{
	m_up = vec3(0.0f,1.0f,0.0f);
	m_lookAt = vec3(0.0f,0.0f,1.0f);
	m_right = vec3(1.0f,0.0f,0.0f);

	mat4 R;

	rotate(R, m_pitch, m_right);
	vec4 temp;

	temp = vec4(m_up, 0.0f) * R;
	m_up = temp.xyz();

	temp = vec4(m_lookAt, 0.0f) * R;
	m_lookAt = temp.xyz();


	rotate(R, m_yaw, vec3(0.0f, 1.0f, 0.0f));


	temp = vec4(m_up, 0.0f) * R;
	m_up = temp.xyz();

	temp = vec4(m_right, 0.0f) * R;
	m_right = temp.xyz();

	temp = vec4(m_lookAt, 0.0f) * R;
	m_lookAt = temp.xyz();


	// Update view matrix.
	m_view[0].x = m_right.x; m_view[0].y = m_up.x; m_view[0].z = m_lookAt.x;
	m_view[1].x = m_right.y; m_view[1].y = m_up.y; m_view[1].z = m_lookAt.y;
	m_view[2].x = m_right.z; m_view[2].y = m_up.z; m_view[2].z = m_lookAt.z;
	 
	m_view[3].x = - dot(m_cameraPos, m_right );
	m_view[3].y = - dot(m_cameraPos, m_up );
	m_view[3].z = - dot(m_cameraPos, m_lookAt );
	 
	m_view[0].w = 0.0f;
	m_view[1].w = 0.0f;
	m_view[2].w = 0.0f;
	m_view[3].w = 1.0f;
}

void Camera::setViewMatrix(vec3 p_pos)
{
	m_up = vec3(0.0f,1.0f,0.0f);
	m_lookAt = vec3(0.0f,0.0f,1.0f);
	m_right = vec3(1.0f,0.0f,0.0f);

	m_cameraPos = p_pos;

	m_pitch = 0;
	m_yaw = 360.0f*(PI/180.0f);

	mat4 R;
	R[1].y = cos(m_pitch);
	R[1].z = sin(m_pitch);
	R[2].y = -sin(m_pitch);
	R[2].z = cos(m_pitch);
	
	vec4 temp;
	temp = vec4(m_up, 0.0f) * R;
	m_up = temp.xyz();
	
	temp = vec4(m_lookAt, 0.0f) * R;
	m_lookAt = temp.xyz();
	

	R = mat4(1.0f);
	R[0].x = cos(m_yaw);
	R[0].z = -sin(m_yaw);
	R[2].x = sin(m_yaw);
	R[2].z = cos(m_yaw);
	

	temp = vec4(m_up, 0.0f) * R;
	m_up = temp.xyz();

	temp = vec4(m_right, 0.0f) * R;
	m_right = temp.xyz();
	
	temp = vec4(m_lookAt, 0.0f) * R;
	m_lookAt = temp.xyz();
	

	// Update view matrix.
	m_view[0].x = m_right.x; m_view[0].y = m_up.x; m_view[0].z = m_lookAt.x;
	m_view[1].x = m_right.y; m_view[1].y = m_up.y; m_view[1].z = m_lookAt.y;
	m_view[2].x = m_right.z; m_view[2].y = m_up.z; m_view[2].z = m_lookAt.z;
	 
	m_view[3].x = - dot(m_cameraPos, m_right );
	m_view[3].y = - dot(m_cameraPos, m_up );
	m_view[3].z = - dot(m_cameraPos, m_lookAt );
	 
	m_view[0].w = 0.0f;
	m_view[1].w = 0.0f;
	m_view[2].w = 0.0f;
	m_view[3].w = 1.0f;
}

void Camera::walk( float p_f )
{
	vec3 moveVector( m_lookAt.x, 0.0f, m_lookAt.z );
	moveVector = normalize(moveVector);
	
	moveVector *= p_f;
	m_velocity += moveVector;
}

void Camera::strafe( float p_f )
{
	m_velocity += m_right * p_f;
}

void Camera::updateCameraPos()
{
	// Move camera.
	m_cameraPos += m_velocity;
	if(m_cameraPos.x < - 100)
		m_cameraPos.x  = -100;
	else if(m_cameraPos.x > 340)
		m_cameraPos.x  = 340;

	if(m_cameraPos.z < - 200)
		m_cameraPos.z  = -200;
	else if(m_cameraPos.z > 200)
		m_cameraPos.z  = 200;

	m_velocity = vec3( 0.0f, 0.0f, 0.0f );

	m_lookAtPos = m_cameraPos + m_lookAt;
}

void Camera::setY( float p_f )
{
	m_cameraPos.y += p_f ;
}

void Camera::updateYaw( float p_d )
{
	m_yaw += p_d;

	if(m_yaw >= 2 * PI)
		m_yaw -= 2*PI;
	else if(m_yaw <= 0.0f )
		m_yaw += 2*PI;
}

void Camera::updatePitch( float p_d )
{
	if(m_pitch < m_MAXpitch) // Can't pitch up more than MAXpitch.
		m_pitch += p_d;
	else
		m_pitch = m_MAXpitch;

	if(m_pitch > m_MINpitch) // Can't pitch down more than MINpitch.
		m_pitch += p_d;
	else
		m_pitch = m_MINpitch;
}

float Camera::cotan( float p_d )
{
	return 1 / tan(p_d);
}

mat4 Camera::getProjectionMatrix()
{
	return m_projection;
}

mat4 Camera::getViewMatrix()
{
	return m_view;
}

vec3 Camera::getPosition()
{
	return m_cameraPos;
}

vec3 Camera::getLookAt()
{
	return m_lookAt;
}

void Camera::setPosition(float p_x, float p_y, float p_z)
{
	m_cameraPos = vec3(p_x, p_y, p_z);
}

float Camera::getFarPlane()
{
	return m_farPlane;
}