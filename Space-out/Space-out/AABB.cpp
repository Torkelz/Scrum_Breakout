#include "AABB.h"

AABB::AABB(vec3 p_top, vec3 p_bot, vec4 p_color) : BoundingVolume()
{
	m_top		= p_top;
	m_bottom	= p_bot;
	
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

	/*if (G_DEBUG)
	{
		buildCubeIndices(0);

		BUFFER_INIT_DESC desc;
		desc.Type				= VERTEX_BUFFER;
		desc.NumElements		= 8;
		desc.ElementSize		= sizeof( D3DXVECTOR3 );
		desc.Usage				= BUFFER_DEFAULT;
		desc.InitData			= m_bounds;
		m_vertexBuffer = new Buffer();
		m_vertexBuffer->init(m_device, desc);

		int						temp[24];
		for (UINT i = 0; i < 24; i++)
		{
			temp[i]				= m_indices.at(i);
		}

		desc.Type				= INDEX_BUFFER;
		desc.NumElements		= 24;
		desc.ElementSize		= sizeof(UINT);
		desc.Usage				= BUFFER_DEFAULT;
		desc.InitData			= temp;

		m_indexBuffer = new Buffer();
		m_indexBuffer->init(m_device, desc);

		m_shader = new Shader();
		m_shader->init(m_device, "BoundingBox.fx", 12);
	}*/
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

	m_center = m_top + m_bottom;
	m_center *= 0.5f;
	m_halfDiagonal = m_top - m_bottom;
	m_halfDiagonal *= 0.5f;

	m_sphere.setRadius(length(m_halfDiagonal));
	m_sphere.updatePosition(m_center);
}

void AABB::updatePosition(mat4 p_scale, mat4 p_translate)
{
	mat4 scalate;
	scalate = p_scale * p_translate;

	vec4 v = vec4(m_bottom, 0.0f) * scalate;
	m_bounds[0].x = v.x;
	m_bounds[0].y = v.y;
	m_bounds[0].z = v.z;

	v = vec4(m_top, 0.0f) * scalate;
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

void AABB::draw(mat4& p_world, mat4& p_view, mat4& p_proj)
{
	/*D3DXMATRIX WorldViewProj;
	D3DXMatrixMultiply( &WorldViewProj, &p_world, &p_view );
	D3DXMatrixMultiply( &WorldViewProj, &WorldViewProj, &p_proj );

	m_device->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_LINELIST);
	m_indexBuffer->apply(0);
	m_vertexBuffer->apply(0);

	D3D10_TECHNIQUE_DESC techDesc;
	m_shader->getTechnique()->GetDesc( &techDesc );
			
	m_shader->setFloat4("gColor", m_color);
	m_shader->setMatrix("g_mWorldViewProjection", WorldViewProj);

	m_shader->apply(0);
	m_device->DrawIndexed(24, 0, 0);*/
}