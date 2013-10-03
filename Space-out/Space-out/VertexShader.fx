cbuffer cb
{
	float4x4 WVP;
};

struct VSinput
{
	float3 pos : POSITION;
	float2 texPos : TEXTUREPOSITION;
};

struct PSinput
{
	float4 pos : SV_POSITION;
	float2 texPos : TEXTUREPOSITION;
};
	
PSinput main( VSinput input)
{
	PSinput output;
	output.pos = mul(float4(input.pos,1.0f), WVP);
	output.texPos = input.texPos;
	return output;
}