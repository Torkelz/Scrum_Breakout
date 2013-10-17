Texture2D m_blockTex : register ( t0 );
Texture2D m_expBlockTex : register ( t1 );
SamplerState m_textureSampler : register ( s0 );

cbuffer BlockConstBuffer
{
	float4x4 m_WVP;
	float4x4 m_rotation;
	float	 m_sizeX;
	float	 m_sizeY;
	float	 m_sizeZ;
	int		 m_state;
};

struct VSInput
{
	float3			m_posL : POSITION;
	unsigned int	m_blockType : BLOCKTYPE;
};

struct VSOutput
{
	float4 m_posH : SV_POSITION;
	unsigned int m_blockType : BLOCKTYPE;
};

struct GSOutput
{
	float4 m_posH : SV_POSITION;
	float2 m_tex  : TEXCOORD;
	unsigned int m_blockType : BLOCKTYPE;
};

VSOutput VS(VSInput p_vIn)
{
	VSOutput vOut;
	
	// Transform to homogeneous clip space.
	vOut.m_posH = mul(float4(p_vIn.m_posL, 1.0f), m_WVP);

    return vOut;
}


[maxvertexcount(24)]
void GS( point VSOutput p_input[1], inout TriangleStream<GSOutput> p_outputStream)
{
	float dimensions = 2.0f;
	float4x4 rWVP = mul(m_rotation, m_WVP);

	float4 vert0 = p_input[0].m_posH + mul( float4(float4(-1.0*m_sizeX, -1.0*m_sizeY, -1.0*m_sizeZ, 0.0)), rWVP); // 0 --- LowerLeftFront
	float4 vert1 = p_input[0].m_posH + mul( float4(float4( 1.0*m_sizeX, -1.0*m_sizeY, -1.0*m_sizeZ, 0.0)), rWVP); // 1 +-- LowerRightFront
	float4 vert2 = p_input[0].m_posH + mul( float4(float4(-1.0*m_sizeX,  1.0*m_sizeY, -1.0*m_sizeZ, 0.0)), rWVP); // 2 -+- UpperLeftFront
	float4 vert3 = p_input[0].m_posH + mul( float4(float4( 1.0*m_sizeX,  1.0*m_sizeY, -1.0*m_sizeZ, 0.0)), rWVP); // 3 ++- UpperRightFront
	float4 vert4 = p_input[0].m_posH + mul( float4(float4(-1.0*m_sizeX, -1.0*m_sizeY,  1.0*m_sizeZ, 0.0)), rWVP); // 4 --+ LowerLeftBack
	float4 vert5 = p_input[0].m_posH + mul( float4(float4( 1.0*m_sizeX, -1.0*m_sizeY,  1.0*m_sizeZ, 0.0)), rWVP); // 5 +-+ LowerRightBack
	float4 vert6 = p_input[0].m_posH + mul( float4(float4(-1.0*m_sizeX,  1.0*m_sizeY,  1.0*m_sizeZ, 0.0)), rWVP); // 6 -++ UpperLeftBack
	float4 vert7 = p_input[0].m_posH + mul( float4(float4( 1.0*m_sizeX,  1.0*m_sizeY,  1.0*m_sizeZ, 0.0)), rWVP); // 7 +++ UpperRightBack


	GSOutput outVertex = (GSOutput)0;

	// Back
	outVertex.m_posH = vert5;
	outVertex.m_tex = float2(1.0f, 1.0f);
	p_outputStream.Append(outVertex);
	outVertex.m_posH = vert7;
	outVertex.m_tex = float2(1.0f, 0.0f);
	p_outputStream.Append(outVertex);
	outVertex.m_posH = vert4;
	outVertex.m_tex = float2(0.0f, 1.0f);
	p_outputStream.Append(outVertex);
	outVertex.m_posH = vert6;
	outVertex.m_tex = float2(0.0f, 0.0f);
	p_outputStream.Append(outVertex);
	p_outputStream.RestartStrip();

	// Front
	outVertex.m_posH = vert3;
	outVertex.m_tex = float2(0.0f, 1.0f);
	p_outputStream.Append(outVertex);
	outVertex.m_posH = vert1;
	outVertex.m_tex = float2(1.0f, 1.0f);
	p_outputStream.Append(outVertex);
	outVertex.m_posH = vert2;
	outVertex.m_tex = float2(0.0f, 0.0f);
	p_outputStream.Append(outVertex);
	outVertex.m_posH = vert0;
	outVertex.m_tex = float2(1.0f, 0.0f);
	p_outputStream.Append(outVertex);
	p_outputStream.RestartStrip();

	// Top
	outVertex.m_posH = vert7;
	outVertex.m_tex = float2(1.0f, 0.0f);
	p_outputStream.Append(outVertex);
	outVertex.m_posH = vert3;
	outVertex.m_tex = float2(1.0f, 1.0f);
	p_outputStream.Append(outVertex);
	outVertex.m_posH = vert6;
	outVertex.m_tex = float2(0.0f, 0.0f);
	p_outputStream.Append(outVertex);
	outVertex.m_posH = vert2;
	outVertex.m_tex = float2(0.0f, 1.0f);
	p_outputStream.Append(outVertex);
	p_outputStream.RestartStrip();

	// Bottom
	outVertex.m_posH = vert1;
	outVertex.m_tex = float2(1.0f, 0.0f);
	p_outputStream.Append(outVertex);
	outVertex.m_posH = vert5;
	outVertex.m_tex = float2(1.0f, 1.0f);
	p_outputStream.Append(outVertex);
	outVertex.m_posH = vert0;
	outVertex.m_tex = float2(0.0f, 0.0f);
	p_outputStream.Append(outVertex);
	outVertex.m_posH = vert4;
	outVertex.m_tex = float2(0.0f, 1.0f);
	p_outputStream.Append(outVertex);
	p_outputStream.RestartStrip();

	// Right
	outVertex.m_posH = vert3;
	outVertex.m_tex = float2(0.0f, 0.0f);
	p_outputStream.Append(outVertex);
	outVertex.m_posH = vert7;
	outVertex.m_tex = float2(1.0f, 0.0f);
	p_outputStream.Append(outVertex);
	outVertex.m_posH = vert1;
	outVertex.m_tex = float2(0.0f, 1.0f);
	p_outputStream.Append(outVertex);
	outVertex.m_posH = vert5;
	outVertex.m_tex = float2(1.0f, 1.0f);
	p_outputStream.Append(outVertex);
	p_outputStream.RestartStrip();

	// Left
	outVertex.m_posH = vert6;
	outVertex.m_tex = float2(0.0f, 0.0f);
	p_outputStream.Append(outVertex);
	outVertex.m_posH = vert2;
	outVertex.m_tex = float2(1.0f, 0.0f);
	p_outputStream.Append(outVertex);
	outVertex.m_posH = vert4;
	outVertex.m_tex = float2(0.0f, 1.0f);
	p_outputStream.Append(outVertex);
	outVertex.m_posH = vert0;
	outVertex.m_tex = float2(1.0f, 1.0f);
	p_outputStream.Append(outVertex);
	p_outputStream.RestartStrip();

}


float4 PS(GSOutput p_input) : SV_Target
{
	float4 temp;

	if(p_input.m_blockType == 0)
		temp = m_blockTex.Sample(m_textureSampler, p_input.m_tex);

	if(p_input.m_blockType == 1)
		temp = m_expBlockTex.Sample(m_textureSampler, p_input.m_tex);

	return temp;
    //float4(1.0f, 1.0f, 1.0f, 1.0f);
}
