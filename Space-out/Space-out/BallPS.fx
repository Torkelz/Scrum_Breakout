Texture2D m_texture;

struct PSSceneIn
{
	float4	pos			: SV_POSITION;
	float2	texturePos	: TEXTUREPOS;
};

//-----------------------------------------------------------------------------------------
// SamplerState: LinearSampler  
//-----------------------------------------------------------------------------------------
SamplerState linearSampler
{
	Filter		= MIN_MAG_MIP_LINEAR;
	AddressU	= Wrap;
	AddressV	= Wrap;
};

float4 main(PSSceneIn p_input) : SV_TARGET
{
	return m_texture.Sample(linearSampler, p_input.texturePos);
}