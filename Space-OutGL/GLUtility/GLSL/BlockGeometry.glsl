#version 400
layout (points) in;
layout (triangle_strip, max_vertices = 28) out;

in vec4 outputColor[];
in mat4 oProj[];
out vec4 colorGem;
out vec2 texCoord;

void main()
{
	float m_sizeX = 3.2f;
	float m_sizeY = 3.7f;
	float m_sizeZ = 3.f;
	
	vec4 vert0 =  gl_in[0].gl_Position + vec4(-1.0f*m_sizeX, -1.0f*m_sizeY, -1.0f*m_sizeZ, 0.0);// * oProj[0]; // 0 --- UpperLeftFront
	vec4 vert1 =  gl_in[0].gl_Position + vec4( 1.0f*m_sizeX, -1.0f*m_sizeY, -1.0f*m_sizeZ, 0.0);// * oProj[0]; // 1 +-- LowerLeftFront
	vec4 vert2 =  gl_in[0].gl_Position + vec4(-1.0f*m_sizeX, 1.0f*m_sizeY, -1.0f*m_sizeZ, 0.0);// * oProj[0]; // 2 -+- UpperRightFront
	vec4 vert3 =  gl_in[0].gl_Position + vec4( 1.0f*m_sizeX, 1.0f*m_sizeY, -1.0f*m_sizeZ, 0.0);// * oProj[0]; // 3 ++- LowerRightFront
	vec4 vert4 =  gl_in[0].gl_Position + vec4(-1.0f*m_sizeX, -1.0f*m_sizeY, 1.0f*m_sizeZ, 0.0);// * oProj[0]; // 4 --+ LowerLeftBack
	vec4 vert5 =  gl_in[0].gl_Position + vec4( 1.0f*m_sizeX, -1.0f*m_sizeY, 1.0f*m_sizeZ, 0.0);// * oProj[0]; // 5 +-+ UpperLeftBack
	vec4 vert6 =  gl_in[0].gl_Position + vec4(-1.0f*m_sizeX, 1.0f*m_sizeY, 1.0f*m_sizeZ, 0.0);// * oProj[0]; // 6 -++ LowerRightBack
	vec4 vert7 =  gl_in[0].gl_Position + vec4( 1.0f*m_sizeX, 1.0f*m_sizeY, 1.0f*m_sizeZ, 0.0);// * oProj[0]; // 7 +++ UpperRightBack
	
	
	
	// Front
	gl_Position  = oProj[0] * vert5;
	colorGem = vec4(0.15f, 0.67f, 0.8f, 1.0f);
	texCoord = vec2(0,0);
	EmitVertex();
	gl_Position  = oProj[0] * vert7;
	colorGem = vec4(0.15f, 0.67f, 0.8f, 1.0f);
	texCoord = vec2(0,1);
	EmitVertex();
	gl_Position  = oProj[0] * vert4;
	colorGem = vec4(0.15f, 0.67f, 0.8f, 1.0f);
	texCoord = vec2(1,0);
	EmitVertex();
	gl_Position  = oProj[0] * vert6;
	colorGem = vec4(0.15f, 0.67f, 0.8f, 1.0f);
	texCoord = vec2(1,1);
	EmitVertex();
	EndPrimitive();
	
	// Back
	gl_Position = oProj[0] * vert3;
	colorGem = vec4(0.5f, 0.5f, 0.5f, 1.0f);
	texCoord = vec2(1,1);
	EmitVertex();
	gl_Position = oProj[0] * vert1;
	colorGem = vec4(0.5f, 0.5f, 0.5f, 1.0f);
	texCoord = vec2(1,0);
	EmitVertex();
	gl_Position  = oProj[0] * vert2;
	colorGem = vec4(0.5f, 0.5f, 0.5f, 1.0f);
	texCoord = vec2(0,1);
	EmitVertex();
	gl_Position  = oProj[0] * vert0;
	colorGem = vec4(0.5f, 0.5f, 0.5f, 1.0f);
	texCoord = vec2(0,0);
	EmitVertex();
	EndPrimitive();
	
	// Top
	gl_Position = oProj[0] * vert7;
	colorGem = vec4(1.0f, 0, 0, 1.0f);
	texCoord = vec2(0,0);
	EmitVertex();
	gl_Position = oProj[0] * vert3;
	colorGem = vec4(1.0f, 0, 0, 1.0f);
	texCoord = vec2(0,1);
	EmitVertex();
	gl_Position = oProj[0] * vert6;
	colorGem = vec4(1.0f, 0, 0, 1.0f);
	texCoord = vec2(1,0);
	EmitVertex();
	gl_Position = oProj[0] * vert2;
	colorGem = vec4(1.0f, 0, 0, 1.0f);
	texCoord = vec2(1,1);
	EmitVertex();
	EndPrimitive();
	
	// Bottom
	gl_Position  = oProj[0] * vert1;
	colorGem = vec4(1.0f, 0, 1.0f, 1.0f);
	texCoord = vec2(0,0);
	EmitVertex();
	gl_Position = oProj[0] * vert5;
	colorGem = vec4(1.0f, 0, 1.0f, 1.0f);
	texCoord = vec2(0,1);
	EmitVertex();
	gl_Position  = oProj[0] * vert0;
	colorGem = vec4(1.0f, 0, 1.0f, 1.0f);
	texCoord = vec2(1,0);
	EmitVertex();
	gl_Position  = oProj[0] * vert4;
	colorGem = vec4(1.0f, 0, 1.0f, 1.0f);
	texCoord = vec2(1,1);
	EmitVertex();
	EndPrimitive();
	
	// Right
	gl_Position  = oProj[0] * vert3;
	colorGem = vec4(0.0f, 0.0f, 0.0f, 1.0f);
	texCoord = vec2(1,1);
	EmitVertex();
	gl_Position  = oProj[0] * vert7;
	colorGem = vec4(0.0f, 0.0f, 0.0f, 1.0f);
	texCoord = vec2(1,0);
	EmitVertex();
	gl_Position  = oProj[0] * vert1;
	colorGem = vec4(0.0f, 0.0f, 0.0f, 1.0f);
	texCoord = vec2(0,1);
	EmitVertex();
	gl_Position  = oProj[0] * vert5;
	colorGem = vec4(0.0f, 0.0f, 0.0f, 1.0f);
	texCoord = vec2(0,0);
	EmitVertex();
	EndPrimitive();
	
	// Left
	gl_Position  = oProj[0] * vert6;
	colorGem = vec4(1.0f, 1.0f, 1.0f, 1.0f);
	texCoord = vec2(0,1);
	EmitVertex();
	gl_Position  = oProj[0] * vert2;
	colorGem = vec4(1.0f, 1.0f, 1.0f, 1.0f);
	texCoord = vec2(0,0);
	EmitVertex();
	gl_Position  = oProj[0] * vert4;
	colorGem = vec4(1.0f, 1.0f, 1.0f, 1.0f);
	texCoord = vec2(1,1);
	EmitVertex();
	gl_Position  = oProj[0] * vert0;
	colorGem = vec4(1.0f, 1.0f, 1.0f, 1.0f);
	texCoord = vec2(1,0);
	EmitVertex();
	
	//gl_Position = oProj[0] * gl_in[0].gl_Position;
	
	EndPrimitive();
		
		/*
		gl_Position = gl_in[0].gl_Position;
		colorGem = outputColor[0];
		EmitVertex();
		
		gl_Position = vec4(gl_in[0].gl_Position.x + 2, gl_in[0].gl_Position.y, gl_in[0].gl_Position.z, 1.0f);
		colorGem = outputColor[0];
		EmitVertex();
		
		gl_Position = vec4(gl_in[0].gl_Position.x + 1, gl_in[0].gl_Position.y + 2, gl_in[0].gl_Position.z, 1.0f);
		colorGem = outputColor[0];
		EmitVertex();
	EndPrimitive();*/
}