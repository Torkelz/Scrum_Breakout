cbuffer BlockConstBuffer
{
	float4x4 m_WVP;
	float	 m_sizeX;
	float	 m_sizeY;
	float	 m_sizeZ;
};

struct VSInput
{
	float3 m_posL : POSITION;
	float4 m_col  : COLOR;
};

struct VSOutput
{
	float4 m_posH : SV_POSITION;
	float4 m_col  : COLOR;
};

struct GSOutput
{
	float4 m_posH : SV_POSITION;
	float4 m_col  : COLOR;
};

VSOutput VS(VSInput p_vIn)
{
	VSOutput vOut, lol;
	
	// Transform to homogeneous clip space.
	vOut.m_posH = mul(float4(p_vIn.m_posL, 1.0f), m_WVP);
	
	// Just pass vertex color into the pixel shader.
    vOut.m_col = p_vIn.m_col;
    //vOut.m_col = float4(1.0f, 0.0f, 0.0f, 1.0f);

    return vOut;
}


[maxvertexcount(24)]
void GS( point VSOutput p_input[1], inout TriangleStream<GSOutput> p_outputStream)
{
	float dimensions = 2.0f;
	float4 vert0 = p_input[0].m_posH + mul( float4(float4(-1.0*m_sizeX, -1.0*m_sizeY, -1.0*m_sizeZ, 0.0)), m_WVP); // 0 --- UpperLeftFront
	float4 vert1 = p_input[0].m_posH + mul( float4(float4( 1.0*m_sizeX, -1.0*m_sizeY, -1.0*m_sizeZ, 0.0)), m_WVP); // 1 +-- LowerLeftFront
	float4 vert2 = p_input[0].m_posH + mul( float4(float4(-1.0*m_sizeX,  1.0*m_sizeY, -1.0*m_sizeZ, 0.0)), m_WVP); // 2 -+- UpperRightFront
	float4 vert3 = p_input[0].m_posH + mul( float4(float4( 1.0*m_sizeX,  1.0*m_sizeY, -1.0*m_sizeZ, 0.0)), m_WVP); // 3 ++- LowerRightFront
	float4 vert4 = p_input[0].m_posH + mul( float4(float4(-1.0*m_sizeX, -1.0*m_sizeY,  1.0*m_sizeZ, 0.0)), m_WVP); // 4 --+ LowerLeftBack
	float4 vert5 = p_input[0].m_posH + mul( float4(float4( 1.0*m_sizeX, -1.0*m_sizeY,  1.0*m_sizeZ, 0.0)), m_WVP); // 5 +-+ UpperLeftBack
	float4 vert6 = p_input[0].m_posH + mul( float4(float4(-1.0*m_sizeX,  1.0*m_sizeY,  1.0*m_sizeZ, 0.0)), m_WVP); // 6 -++ LowerRightBack
	float4 vert7 = p_input[0].m_posH + mul( float4(float4( 1.0*m_sizeX,  1.0*m_sizeY,  1.0*m_sizeZ, 0.0)), m_WVP); // 7 +++ UpperRightBack


	GSOutput outVertex = (GSOutput)0;
	outVertex.m_col = p_input[0].m_col;

	// left
	outVertex.m_posH = vert5;
	p_outputStream.Append(outVertex);
	outVertex.m_posH = vert7;
	p_outputStream.Append(outVertex);
	outVertex.m_posH = vert4;
	p_outputStream.Append(outVertex);
	outVertex.m_posH = vert6;
	p_outputStream.Append(outVertex);
	p_outputStream.RestartStrip();

	// Right
	outVertex.m_posH = vert3;
	p_outputStream.Append(outVertex);
	outVertex.m_posH = vert1;
	p_outputStream.Append(outVertex);
	outVertex.m_posH = vert2;
	p_outputStream.Append(outVertex);
	outVertex.m_posH = vert0;
	p_outputStream.Append(outVertex);
	p_outputStream.RestartStrip();

	// Top
	outVertex.m_posH = vert7;
	p_outputStream.Append(outVertex);
	outVertex.m_posH = vert3;
	p_outputStream.Append(outVertex);
	outVertex.m_posH = vert6;
	p_outputStream.Append(outVertex);
	outVertex.m_posH = vert2;
	p_outputStream.Append(outVertex);
	p_outputStream.RestartStrip();

	// Bottom
	outVertex.m_posH = vert1;
	p_outputStream.Append(outVertex);
	outVertex.m_posH = vert5;
	p_outputStream.Append(outVertex);
	outVertex.m_posH = vert0;
	p_outputStream.Append(outVertex);
	outVertex.m_posH = vert4;
	p_outputStream.Append(outVertex);
	p_outputStream.RestartStrip();

	// Front
	outVertex.m_posH = vert3;
	p_outputStream.Append(outVertex);
	outVertex.m_posH = vert7;
	p_outputStream.Append(outVertex);
	outVertex.m_posH = vert1;
	p_outputStream.Append(outVertex);
	outVertex.m_posH = vert5;
	p_outputStream.Append(outVertex);
	p_outputStream.RestartStrip();

	// Back
	outVertex.m_posH = vert6;
	p_outputStream.Append(outVertex);
	outVertex.m_posH = vert2;
	p_outputStream.Append(outVertex);
	outVertex.m_posH = vert4;
	p_outputStream.Append(outVertex);
	outVertex.m_posH = vert0;
	p_outputStream.Append(outVertex);
	p_outputStream.RestartStrip();

}


float4 PS(GSOutput p_input) : SV_Target
{
    return p_input.m_col; //float4(1.0f, 1.0f, 1.0f, 1.0f);
}
