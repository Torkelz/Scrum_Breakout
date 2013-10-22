#ifndef SPHERE_H
#define SPHERE_H

#include "BoundingVolume.h"
#include "Buffer.h"
#include "Shader.h"

////DEBUGGING
struct CBS
{
	XMMATRIX WVP;
	XMFLOAT4 color;
};

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

	////DEBUGGING
	void				initDraw(ID3D11Device* p_pDevice, ID3D11DeviceContext* p_pDeviceContext);
	void				draw( XMMATRIX& p_world, XMMATRIX& p_view, XMMATRIX& p_proj );

private:
	float	m_radius;
	float	m_sqrRadius;

	////DEBUGGING
	Buffer*				m_pBuffer;
	Buffer*				m_pCB;
	Shader*				m_pShader;
	ID3D11Device*		m_pDevice;
	ID3D11DeviceContext* m_pDeviceContext;
	CBS					m_cb;
	XMMATRIX			m_translate;
};

#endif SPHERE_H