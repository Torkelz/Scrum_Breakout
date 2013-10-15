TextureCube gCubeMap;

TextureCube m_texture : register ( t0 );
SamplerState m_textureSampler : register ( s0 );

//-----------------------------------------------------------------------------------------
// Input and Output Structures
//-----------------------------------------------------------------------------------------
struct VSSceneIn
{
	float3 posL : POSITION;
};

struct PSSceneIn
{
	float4 Pos  : SV_Position;		// SV_Position is a (S)ystem (V)ariable that denotes transformed position
    float3 texC : TEXCOORD;
};
//-----------------------------------------------------------------------------------------
// Constant Buffers (where we store variables by frequency of update)
//-----------------------------------------------------------------------------------------
cbuffer cbEveryFrame
{
	matrix	g_mWorldViewProjection;
};

//-----------------------------------------------------------------------------------------
// State Structures
//-----------------------------------------------------------------------------------------
DepthStencilState EnableDepth
{
    DepthEnable = TRUE;
    DepthWriteMask = ALL;
};
BlendState NoBlend
{
	BlendEnable[0] = FALSE;
};
RasterizerState NoCulling
{
	FillMode = SOLID;//WIREFRAME;
	CullMode = NONE;
};

SamplerState linearSampler
{
	Filter = ANISOTROPIC;
	AddressU = Wrap;
	AddressV = Wrap;
};
//-----------------------------------------------------------------------------------------
// VertexShader: VSScene
//-----------------------------------------------------------------------------------------
PSSceneIn VSScene(VSSceneIn input)
{
	PSSceneIn output = (PSSceneIn)0;
	
	// transform the point into view space
	output.Pos = mul( float4(input.posL,1.0), g_mWorldViewProjection );


	output.texC = input.posL;
	
	return output;
}
//-----------------------------------------------------------------------------------------
// PixelShader: PSSceneMain
//-----------------------------------------------------------------------------------------
float4 PSScene(PSSceneIn input) : SV_Target
{	
	//return gCubeMap.Sample(linearSampler, input.texC);
	return m_texture.Sample(m_textureSampler, input.texC);
}

//-----------------------------------------------------------------------------------------
// Technique: RenderTextured  
//-----------------------------------------------------------------------------------------
technique10 RenderSky 
{
    pass p0
    {
		// Set VS, GS, and PS
        SetVertexShader( CompileShader( vs_5_0, VSScene() ) );
        SetGeometryShader( NULL );
        SetPixelShader( CompileShader( ps_5_0, PSScene() ) );
	    
	    SetRasterizerState( NoCulling );
		SetBlendState(NoBlend, float4(0.0f,0.0f,0.0f,0.0f), 0xFFFFFFFF);
	    SetDepthStencilState( EnableDepth, 0 );
    }  
}