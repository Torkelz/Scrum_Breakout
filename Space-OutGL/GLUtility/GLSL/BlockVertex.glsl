#version 400
layout(location = 0) in vec3 in_position;

out vec4 outputTex;
out mat4 oProj;

uniform uniBlock
{
//	vec4 pos;
//	vec4 tex;
	mat4x4 gWorld;
};

void main()
{
	//outputTex = tex;
	gl_Position = vec4(in_position, 1.0f);
	oProj = gWorld;
}