#include "Camera.h"

Camera::Camera(){}

Camera::~Camera(){}

void Camera::init(vec3 initPos)
{
	m_cameraPos	= initPos;
	m_velocity	= vec3( 0.0f, 0.0f, 0.0f );

	m_right		= vec3( 1.0f, 0.0f, 0.0f );
	m_up			= vec3( 0.0f, 1.0f, 0.0f );
	m_lookAt		= vec3( 0.0f, 0.0f, 1.0f );

	m_pitch		= 10.0f;
	m_yaw		= 0.0f;
	m_lookAtPos	= m_cameraPos + m_lookAt;

	m_view = mat4(1.0f);
	m_projection = mat4(1.0f);
	
	m_MAXpitch	= PI/2.0f;
	m_MINpitch	= -PI/2.0f;

	m_lifeTime = 0;
	m_isCinematic = false;
	m_runOnce = false;

	m_angleToNextPF = 90.0f;
}

void Camera::createProjectionMatrix( float p_FOV, float p_aspect,
									float p_nearPlane, float p_farPlane )
{
	m_FOV		= p_FOV;
	m_aspect	= p_aspect;
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
void Camera::createOrthoMatrix(float p_width, float p_height, float p_nearPlane, float p_farPlane)
{
	m_orthoMatrix = mat4(1.0f);

	m_orthoMatrix[0].x = 2.0f/p_width;
	m_orthoMatrix[1].y = 2.0f/p_height;
	m_orthoMatrix[2].z = 1.0f/(p_farPlane - p_nearPlane);
	m_orthoMatrix[3].z = -p_nearPlane/(p_farPlane - p_nearPlane);
}

void Camera::updateViewMatrix()
{
	m_up = vec3(0.0f,1.0f,0.0f);
	m_lookAt = vec3(0.0f,0.0f,1.0f);
	m_right = vec3(1.0f,0.0f,0.0f);

	//m_yaw = 270.0f*(PI/180.0f);

	//setYaw(3);
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

void Camera::setViewMatrix()
{
	m_up = vec3(0.0f,1.0f,0.0f);
	m_lookAt = vec3(0.0f,0.0f,1.0f);
	m_right = vec3(1.0f,0.0f,0.0f);
	
	m_pitch *= (PI/180.0f);

	m_yawStart = 180.0f*(PI/180.0f);
	m_yaw = m_yawStart;
	m_yawNext = m_yawStart;
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
	R[0].x = cos(m_yawStart);
	R[0].z = -sin(m_yawStart);
	R[2].x = sin(m_yawStart);
	R[2].z = cos(m_yawStart);
	

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

void Camera::updateCameraPos(float p_dt)
{
	// Move camera.
	/*m_cameraPos += m_velocity;
	if(m_cameraPos.x < - 100)
		m_cameraPos.x  = -100;
	else if(m_cameraPos.x > 340)
		m_cameraPos.x  = 340;

	if(m_cameraPos.z < - 200)
		m_cameraPos.z  = -200;
	else if(m_cameraPos.z > 200)
		m_cameraPos.z  = 200;

	m_velocity = vec3( 0.0f, 0.0f, 0.0f );*/
/*	if(m_yaw < 0.0f)
		m_yaw += (2*PI);
	if(m_yaw > 2*PI)
		m_yaw -= 2*PI;*/ 

	if(m_isCinematic)
	{
		m_lifeTime += p_dt * 2.f;

		if((unsigned int)m_lifeTime > m_cinematicPos.size() - 2)
			resetCinematic();
		else
		{
			updateViewMatrix();
			interpolateYaw(m_lifeTime);
			flyCinematic(m_lifeTime);
		}
	}
	else
	{
		m_lifeTime = 0;
	}
	//m_lookAtPos = m_cameraPos + m_lookAt;
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

bool Camera::isCinematic()
{
	return m_isCinematic;
}

void Camera::startCinematic()
{
	m_isCinematic = true;
}

bool Camera::timeToChange()
{
	//if((m_lifeTime / m_cinematicPos.size() - 2) > 0.5f)
	//	return true;

	return (m_lifeTime / (m_cinematicPos.size() - 2)) > 0.5f ? true : false;
}

bool Camera::getRunOnce()
{
	return m_runOnce;
}

void Camera::setRunOnce(bool p)
{
	m_runOnce = p;
}

void Camera::buildPath(vec3 p_start, vec3 p_stop, vec3 p_originWorld, int p_nrPoints)
{
	m_cinematicPos.push_back(p_start);

	vec3 dirStart = normalize(p_start - p_originWorld);
	vec3 dirStop = normalize(p_stop - p_originWorld);
	vec3 zoomOut = (p_start - p_originWorld) + dirStart * 30.f;

	vec3 checkAngle = cross(dirStart, dirStop);

	//float angle = 90.f;//(PI/180.0f);

	if(checkAngle.y > 0) // Test if its right, rotating right direction
		m_angleToNextPF = -90.0f;
	else if(checkAngle.y < 0) // Test if its right, rotating right direction
		m_angleToNextPF = 90.0f;
	
	mat4 rot = rotate(mat4(1.0f), -m_angleToNextPF/p_nrPoints, vec3(0.0f, 1.0f, 0.0f));
	
	vec4 tempZoomOut;

	tempZoomOut = vec4(zoomOut, 0.0f);

	for(int i = 0; i < p_nrPoints-1; i++)
	{
		tempZoomOut = rot * tempZoomOut;

		m_cinematicPos.push_back(p_originWorld + tempZoomOut.xyz());
	}
	
	//m_cinematicPos.push_back(p_originWorld + ((p_stop - p_originWorld) + (dirStop * 10.f)));


	m_cinematicPos.push_back(p_stop);
}

void Camera::flyCinematic(float p_lifeTime)
{
	float proc = p_lifeTime - (int)p_lifeTime;

	vec3 p0, p4;

	if((unsigned int)p_lifeTime == 0)
		p0 = m_cinematicPos.at((unsigned int)p_lifeTime);
	else
		p0 = m_cinematicPos.at((unsigned int)p_lifeTime - 1);


	if((unsigned int)p_lifeTime + 2 > m_cinematicPos.size() - 1)
		p4 = m_cinematicPos.at((unsigned int)p_lifeTime + 1);
	else
		p4 = m_cinematicPos.at((unsigned int)p_lifeTime + 2);

	m_cameraPos = catmullRom(	p0, 
								m_cinematicPos.at((unsigned int)p_lifeTime), 
								m_cinematicPos.at((unsigned int)p_lifeTime + 1), 
								p4,
								proc);
}

void Camera::interpolateYaw(float p_lifeTime)
{
	float pc = (p_lifeTime / (m_cinematicPos.size() - 1));
	m_yaw = m_yawStart + ((m_yawNext - m_yawStart) * pc);
}

void Camera::resetCinematic()
{
	m_cinematicPos.clear();
	m_isCinematic = false;
}

float Camera::cotan( float p_d )
{
	return 1 / tan(p_d);
}

mat4 Camera::getProjectionMatrix()
{
	return m_projection;
}

mat4 Camera::getOrthoMatrix()
{
	return m_orthoMatrix;
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

void Camera::setYaw(int p_activePlayField)
{
	m_yawStart = m_yaw;

	//switch (p_activePlayField)
	//{
	//case 0:
	//	if(m_yawNext == 90.0f * PI/180.0f)
	//	{
	//		m_yawNext  = -180.0f;
	//		break;
	//	}
	//	m_yawNext = 180.0f;
	//	break;

	//case 1:
	//	if(m_yawNext == 360.0f * PI/180.0f)
	//	{
	//		m_yawNext = 450.0f;
	//		break;
	//	}
	//	m_yawNext = 90.0f;
	//	break;

	//case 2:
	//	
	//	if(m_yawNext == (270.0f * PI/180.0f))
	//	{
	//		m_yawNext = 360.0f;
	//		break;
	//	}
	//	m_yawNext = 0.0f;
	//	break;

	//case 3:
	//	//if(m_yawNext == (180.0f * PI/180.0f))//0.0174532925f))
	//	if(m_yawNext == 0.0f)
	//	{
	//		m_yawNext = -90.0f;
	//		break;
	//	}
	//	m_yawNext = 270.0f;
	//	break;

	//default:
	//	break;
	//}


	m_yawNext += m_angleToNextPF * (PI/180.0f);
}

void Camera::setPosition(float p_x, float p_y, float p_z)
{
	m_cameraPos = vec3(p_x, p_y, p_z);
}

float Camera::getFarPlane()
{
	return m_farPlane;
}

mat4 Camera::getViewMatrixSpecYaw(float p_yaw)
{
	m_up = vec3(0.0f,1.0f,0.0f);
	m_lookAt = vec3(0.0f,0.0f,1.0f);
	m_right = vec3(1.0f,0.0f,0.0f);

	mat4 ret;

	//setYaw(3);
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
	R[0].x = cos(p_yaw);
	R[0].z = -sin(p_yaw);
	R[2].x = sin(p_yaw);
	R[2].z = cos(p_yaw);
	

	temp = vec4(m_up, 0.0f) * R;
	m_up = temp.xyz();

	temp = vec4(m_right, 0.0f) * R;
	m_right = temp.xyz();
	
	temp = vec4(m_lookAt, 0.0f) * R;
	m_lookAt = temp.xyz();


	// Update view matrix.
	ret[0].x = m_right.x; ret[0].y = m_up.x; ret[0].z = m_lookAt.x;
	ret[1].x = m_right.y; ret[1].y = m_up.y; ret[1].z = m_lookAt.y;
	ret[2].x = m_right.z; ret[2].y = m_up.z; ret[2].z = m_lookAt.z;
	 
	ret[3].x = - dot(m_cameraPos, m_right );
	ret[3].y = - dot(m_cameraPos, m_up );
	ret[3].z = - dot(m_cameraPos, m_lookAt );
	 
	ret[0].w = 0.0f;
	ret[1].w = 0.0f;
	ret[2].w = 0.0f;
	ret[3].w = 1.0f;

	return ret;
}