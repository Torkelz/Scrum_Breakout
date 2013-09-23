
Texture2D m_texture;

struct temp
{
	float4 pos;
};

cbuffer	cb
{
	float4		eyePosW;
	float4x4	viewProj;
	float4x4	translation;
	float2		size;
};

cbuffer cbFixed
{
	// Define where to bind the texture
	float2 quadTexC[4] = 
	{
		float2( 0.0f, 1.0f ),
		float2( 1.0f, 1.0f ),
		float2( 0.0f, 0.0f ),
		float2( 1.0f, 0.0f )
	};
};

struct PSSceneIn
{
	float4	pos			: SV_POSITION;
	float2	texturePos	: TEXTUREPOS;
};

float4 VShader( float3 pos : POSITION ) : SV_POSITION
{
	return float4(pos, 0.0f);
}

// The draw GS just expands points into camera facing quads.
[maxvertexcount(4)]
void GShader(point temp gIn[1] : SV_POSITION, inout TriangleStream<PSSceneIn> triStream)
{
	// Compute world matrix so that billboard faces the camera.
	float3 t_pos;
	t_pos.x = gIn[0].pos.x;
	t_pos.y = gIn[0].pos.y;
	t_pos.z = gIn[0].pos.z;
	float3 look = normalize(eyePosW.xyz - t_pos);
	float3 right = normalize(cross(float3(0,1,0), look));
	float3 up = float3( 0.0f, 1.0f, 0.0f ); // to make the particles turn around the y-axis
	float4x4 world;
	world[0] = float4(right, 0.0f);
	world[1] = float4(up, 0.0f);
	world[2] = float4(look, 0.0f);
	world[3] = float4(gIn[0].pos.xyz, 1.0f);
	float4x4 WVP = mul(translation, world);
	WVP = mul(WVP, viewProj);

	// Compute 4 triangle strip vertices (quad) in local space.
	// The quad faces down the +z-axis in local space.
	float halfWidth = 0.5f*size.x;
	float halfHeight = 0.5f*size.y;
	float4 v[4];
	v[0] = float4(-halfWidth, -halfHeight, 0.0f, 1.0f);
	v[1] = float4(+halfWidth, -halfHeight, 0.0f, 1.0f);
	v[2] = float4(-halfWidth, +halfHeight, 0.0f, 1.0f);
	v[3] = float4(+halfWidth, +halfHeight, 0.0f, 1.0f);

	// Transform quad vertices to world space and output
	// them as a triangle strip.
	PSSceneIn gOut;
	[unroll]

	for(int i = 0; i < 4; ++i)
	{
		gOut.pos	= mul(v[i], WVP);
		gOut.texturePos	= quadTexC[i];
		triStream.Append(gOut);
	}
}

//-----------------------------------------------------------------------------------------
// SamplerState: LinearSampler  
//-----------------------------------------------------------------------------------------
SamplerState linearSampler
{
	Filter		= MIN_MAG_MIP_LINEAR;
	AddressU	= Wrap;
	AddressV	= Wrap;
};

float4 PShader(PSSceneIn p_input) : SV_TARGET
{
	return float4(0.0f, 0.0f, 0.0f, 1.0f);
	//return m_texture.Sample(linearSampler, p_input.texturePos);
}