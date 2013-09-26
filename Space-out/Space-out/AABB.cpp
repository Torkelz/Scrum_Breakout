#include "AABB.h"

AABB::AABB(vec3 p_top, vec3 p_bot, vec4 p_color) : BoundingVolume()
{
	m_top		= p_top;// + vec3(1.0f, 1.0f, 0.0f);
	m_bottom	= p_bot;// + vec3(-1.0f, -1.0f, 0.0f);
	
	m_bounds[0] = m_bottom;
	m_bounds[7] = m_top;

	m_position = vec3(	m_bottom.x + ((m_top.x - m_bottom.x) / 2) , 
								m_bottom.y + ((m_top.y - m_bottom.y) / 2) , 
								m_bottom.z + ((m_top.z - m_bottom.z) / 2) );

	m_color		= p_color;
	m_type		= AABBOX;

	initialize();
}
AABB::~AABB(){}

void AABB::initialize()
{
	calculateBounds();
}

void AABB::calculateBounds()
{
	m_position = vec3(	m_bounds[0].x + ((m_bounds[7].x - m_bounds[0].x) / 2) , 
						m_bounds[0].y + ((m_bounds[7].y - m_bounds[0].y) / 2) , 
						m_bounds[0].z + ((m_bounds[7].z - m_bounds[0].z) / 2) );

	m_bounds[1] = vec3( m_bounds[7].x,		m_bounds[0].y,		m_bounds[0].z ); // Xyz
	m_bounds[2] = vec3( m_bounds[0].x,		m_bounds[7].y,		m_bounds[0].z ); // xYz
	m_bounds[3] = vec3( m_bounds[7].x,		m_bounds[7].y,		m_bounds[0].z ); // XYz
	m_bounds[4] = vec3( m_bounds[0].x,		m_bounds[0].y,		m_bounds[7].z ); // xyZ
	m_bounds[5] = vec3( m_bounds[7].x,		m_bounds[0].y,		m_bounds[7].z ); // XyZ
	m_bounds[6] = vec3( m_bounds[0].x,		m_bounds[7].y,		m_bounds[7].z ); // xYZ

	m_distances[0] = m_top.y - m_bottom.y;
	m_distances[1] = m_top.x - m_bottom.x;
	m_distances[2] = m_top.z - m_bottom.z;

	m_halfDiagonal = m_bounds[7] - m_bounds[0];
	m_halfDiagonal *= 0.5f;

	m_sphere.setRadius(length(m_halfDiagonal));
	m_sphere.updatePosition(m_position);
}

void AABB::updatePosition(mat4 p_scale, mat4 p_translate)
{
	mat4 scalate;
	scalate = p_scale * p_translate;

	/*for (int i = 0; i < 4; i++)
	{
		m_translate.r[i].m128_f32[0] = scalate[i].x;
		m_translate.r[i].m128_f32[1] = scalate[i].y;
		m_translate.r[i].m128_f32[2] = scalate[i].z;
		m_translate.r[i].m128_f32[3] = scalate[i].w;
	}*/

	scalate = transpose(scalate);
	
	vec4 v = vec4(m_bottom, 1.0f) * scalate;
	m_bounds[0].x = v.x;
	m_bounds[0].y = v.y;
	m_bounds[0].z = v.z;

	v = vec4(m_top, 1.0f) * scalate;
	m_bounds[7].x = v.x;
	m_bounds[7].y = v.y;
	m_bounds[7].z = v.z;

	calculateBounds();
}

void AABB::buildCubeIndices(int offset)
{
	// Cube Composition
	/*		  +----+  6    7	| Refered to as
			 /    /|			| indices of the
			+----+ |  4    5	| vertices below
	2	 3	|    | +			
			|    |/				
	0	 1	+----+								*/

	// Right/Left
	for( int i = 0; i < 8; i++ )
	{
		m_indices.push_back( i + offset );
	}
	// Froward/Backward
	for( int i = 0; i < 4; i++ )
	{
		m_indices.push_back( i + offset );
		m_indices.push_back( i + offset + 4 );
	}
	// Up/Down
	for( int i = 0; i < 2; i++ )
	{
		m_indices.push_back( i + offset );
		m_indices.push_back( i + offset + 2 );
		m_indices.push_back( i + offset + 4 );
		m_indices.push_back( i + offset + 2 + 4 );
	}
}

vec3* AABB::getMax()
{
	return &m_bounds[7];
}

vec3* AABB::getMin()
{
	return &m_bounds[0];
}

int AABB::boxVsBox( AABB* p_box )
{
	vec3* max = p_box->getMax();
	vec3* min = p_box->getMin();

	if ( getMin()->x > max->x || min->x > getMax()->x )
		return OUTSIDE;
	if ( getMin()->y > max->y || min->y > getMax()->y )
		return OUTSIDE;
	if ( getMin()->z > max->z || min->z > getMax()->z )
		return OUTSIDE;

	return INTERSECT;
}

bool AABB::boxVsSphere(Sphere* p_pSphere)
{
	if (m_sphere.sphereVsSphere(p_pSphere))
	{
		//Check to see if the sphere overlaps the AABB
		//const bool AABBOverlapsSphere ( const AABB& B, const SCALAR r, VECTOR& C )
		float s, d = 0; 

		//find the square of the distance
		//from the sphere to the box

		// x
		if( p_pSphere->getPosition()->x < m_bounds[0].x )
		{
			s = p_pSphere->getPosition()->x - m_bounds[0].x;
			d += s*s;
		}
		else if( p_pSphere->getPosition()->x > m_bounds[7].x )
		{
			s = p_pSphere->getPosition()->x - m_bounds[7].x;
			d += s*s;
		}

		// y
		if( p_pSphere->getPosition()->y < m_bounds[0].y )
		{
			s = p_pSphere->getPosition()->y - m_bounds[0].y;
			d += s*s;
		}
		else if( p_pSphere->getPosition()->y > m_bounds[7].y )
		{
			s = p_pSphere->getPosition()->y - m_bounds[7].y;
			d += s*s;
		}

		// z
		if( p_pSphere->getPosition()->z < m_bounds[0].z )
		{
			s = p_pSphere->getPosition()->z - m_bounds[0].z;
			d += s*s;
		}
		else if( p_pSphere->getPosition()->z > m_bounds[7].z )
		{
			s = p_pSphere->getPosition()->z - m_bounds[7].z;
			d += s*s;
		}

		return d <= p_pSphere->getSqrRadius();
	}
	return false;
}

bool AABB::collide(BoundingVolume* p_pVolume)
{
	if(p_pVolume->getType() == AABBOX)
	{
		return boxVsBox((AABB*)p_pVolume);
	}
	else if(p_pVolume->getType() == SPHERE)
	{
		return boxVsSphere((Sphere*)p_pVolume);
	}

	return false;
}

vec3 AABB::findNewDirection(vec3 p_sphereCenter, vec3 p_speed)
{
	vec3 returnVector;
	float speed;
	vec3 t_centerVector;

	int plane = findPlane(p_sphereCenter);

	switch(plane)
	{
		case TOP:
		case BOTTOM:
			returnVector = vec3(p_speed.x, -p_speed.y, p_speed.z);
			break;

		case LEFT:
		case RIGHT:
			returnVector = vec3(-p_speed.x, p_speed.y, -p_speed.z);
			break;

		case CORNER:
			t_centerVector = p_sphereCenter - m_position;
			t_centerVector = normalize(t_centerVector);
			speed = length(p_speed);
			returnVector = speed * (t_centerVector + p_speed);
			break;

		default:
			break;
	};

	return returnVector;
}

int AABB::findPlane(vec3 p_sphereCenter)
{
	vec3 t_centerVec = normalize(p_sphereCenter - m_position);
	vec3 t_up = vec3(0.0f, 1.0f, 0.0f);

	float angle = acos(dot(t_centerVec, t_up) / (length(t_centerVec) * length(t_up)) );
	
	if(angle >= 5.5850536f || angle <= 0.6981317f) // More than 320 or less than 40 degrees. 0,0174532925
		return TOP;
	if(angle >= 0.872664625f && angle <= 2.0943951f) // More than 50 and less than 130 degrees.
		return RIGHT;
	if(angle >= 2.44346095f && angle <= 3.83972435f) // More than 140 and less than 220 degrees.
		return BOTTOM;
	if(angle >= 4.014257275f && angle <= 5.410520675f) // More than 230 and less than 310
		return LEFT;
	if(	angle > 0.6981317f && angle < 0.872664625f ||
		angle > 2.0943951f && angle < 2.44346095f ||
		angle > 3.83972435f && angle < 4.014257275f ||
		angle > 5.410520675f && angle < 5.5850536f )
		return CORNER;

	return -1;
}

//void AABB::calculateCornerVectors()
//{
//	m_cornerVectors[0] = ( m_bounds[0] + ((m_bounds[1] - m_bounds[0]) * 0.5f) ) - m_position;
//	m_cornerVectors[1] = ( m_bounds[0] + ((m_bounds[4] - m_bounds[0]) * 0.5f) ) - m_position;
//	m_cornerVectors[2] = ( m_bounds[4] + ((m_bounds[5] - m_bounds[4]) * 0.5f) ) - m_position;
//	m_cornerVectors[3] = ( m_bounds[1] + ((m_bounds[5] - m_bounds[1]) * 0.5f) ) - m_position;
//	m_cornerVectors[4] = ( m_bounds[2] + ((m_bounds[3] - m_bounds[2]) * 0.5f) ) - m_position;
//	m_cornerVectors[5] = ( m_bounds[2] + ((m_bounds[6] - m_bounds[2]) * 0.5f) ) - m_position;
//	m_cornerVectors[6] = ( m_bounds[6] + ((m_bounds[7] - m_bounds[6]) * 0.5f) ) - m_position;
//	m_cornerVectors[7] = ( m_bounds[3] + ((m_bounds[7] - m_bounds[3]) * 0.5f) ) - m_position;
//}

//void AABB::initDraw(ID3D11Device* p_pDevice, ID3D11DeviceContext* p_pDeviceContext)
//{
//	m_pDevice = p_pDevice;
//	m_pDeviceContext = p_pDeviceContext;
//
//	m_sphere.initDraw(m_pDevice, m_pDeviceContext);
//
//	buildCubeIndices(0);
//
//	BufferInitDesc desc;
//	desc.type				= VERTEX_BUFFER;
//	desc.numElements		= 8;
//	desc.elementSize		= sizeof( XMFLOAT3 );
//	desc.usage				= BUFFER_DEFAULT;
//	desc.initData			= m_bounds;
//
//	m_pBuffer = new Buffer();
//	m_pBuffer->init(p_pDevice, p_pDeviceContext, desc);
//
//	BufferInitDesc cbDesc;	
//
//	cbDesc.elementSize = sizeof(CB);
//	cbDesc.initData = NULL;
//	cbDesc.numElements = 1;
//	cbDesc.type = CONSTANT_BUFFER_VS;
//	cbDesc.usage = BUFFER_DEFAULT;
//	
//	m_pCB = new Buffer();
//	m_pCB->init(p_pDevice, p_pDeviceContext, cbDesc);
//
//	int						temp[24];
//	for (UINT i = 0; i < 24; i++)
//	{
//		temp[i]				= m_indices.at(i);
//	}
//
//	desc.type				= INDEX_BUFFER;
//	desc.numElements		= 24;
//	desc.elementSize		= sizeof(UINT);
//	desc.usage				= BUFFER_DEFAULT;
//	desc.initData			= &temp;
//
//	m_pIndexBuffer = new Buffer();
//	m_pIndexBuffer->init(p_pDevice, p_pDeviceContext, desc);
//
//
//
//	D3D11_INPUT_ELEMENT_DESC inputDesc[] = 
//	{
//		{"POS", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0}
//	};
//
//	m_pShader = new Shader();
//	m_pShader->init(p_pDevice, p_pDeviceContext, 1);
//	m_pShader->compileAndCreateShaderFromFile(L"BoundingBox.fx", "VSInstmain","vs_5_0", VERTEX_SHADER , inputDesc);
//	m_pShader->compileAndCreateShaderFromFile(L"BoundingBox.fx", "PSScene", "ps_5_0", PIXEL_SHADER, NULL);
//}
//
//void AABB::draw(XMMATRIX& p_world, XMMATRIX& p_view, XMMATRIX& p_proj)
//{
//	XMMATRIX WorldViewProj;
//	WorldViewProj = XMMatrixMultiply(p_world, m_translate);
//	WorldViewProj = XMMatrixMultiply(WorldViewProj, p_view );
//	WorldViewProj = XMMatrixMultiply(WorldViewProj, p_proj );
//
//	m_pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
//
//	m_cb.color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
//	m_cb.WVP = XMMatrixTranspose(WorldViewProj);
//	m_pCB->apply(0);
//	m_pDeviceContext->UpdateSubresource(m_pCB->getBufferPointer(), 0, NULL, &m_cb, 0, 0);
//	
//	m_pIndexBuffer->apply(0);
//	m_pBuffer->apply(0);
//	m_pShader->setShaders();
//
//	m_pDeviceContext->DrawIndexed(24, 0, 0);
//	
//	m_sphere.draw(p_world, p_view, p_proj);
//}
