//-----------------------------------------------------------------------------------------
// Input and Output Structures
//-----------------------------------------------------------------------------------------
struct VSSceneIn
{
	float3 Pos	: POS;
};

struct PSSceneIn
{
	float4 Pos  : SV_Position;		// SV_Position is a (S)ystem (V)ariable that denotes transformed position
	float4 Color : POS;
};

//-----------------------------------------------------------------------------------------
// Constant Buffers (where we store variables by frequency of update)
//-----------------------------------------------------------------------------------------
cbuffer CB
{
	matrix	WVP;
	float4	color;
};

//-----------------------------------------------------------------------------------------
// VertexShader: VSInstmain
//-----------------------------------------------------------------------------------------
PSSceneIn VSInstmain(VSSceneIn input)
{
	PSSceneIn output = (PSSceneIn)0;
	
	// transform the point into view space
	output.Pos = mul(float4(input.Pos,1.0), WVP );

	float4 t = output.Pos;

	output.Color = color;

	return output;
}

//-----------------------------------------------------------------------------------------
// PixelShader: PSSceneMain
//-----------------------------------------------------------------------------------------
float4 PSScene(PSSceneIn input) : SV_Target
{
	return input.Color;
}