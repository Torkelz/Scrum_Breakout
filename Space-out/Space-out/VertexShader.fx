cbuffer cb
{
	float4x4 WVP;
}
	
float4 main( float3 pos : POSITION ) : SV_POSITION
{
	return mul(float4(pos,1.0f),WVP);
}