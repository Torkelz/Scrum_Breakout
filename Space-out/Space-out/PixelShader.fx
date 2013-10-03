Texture2D m_texture : register ( t0 );
SamplerState m_textureSampler : register ( s0 );

struct PSinput
{
	float4 pos : SV_POSITION;
	float2 texPos : TEXTUREPOSITION;
};

float4 main(PSinput input) : SV_TARGET
{
	float4 temp = m_texture.Sample(m_textureSampler, input.texPos);
	return temp;
	//return float4(1.0f, 0.0f, 0.0f, 1.0f);
}