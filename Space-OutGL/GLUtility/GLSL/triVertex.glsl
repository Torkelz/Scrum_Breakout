#version 400
layout(location = 0) in vec3 in_position;
layout(location = 1) in vec3 in_color;

out vec4 outputColor;
out mat4 oProj;

uniform ColorBlock
{
	vec4 pos;
	vec4 color;
	mat4x4 gWorld;
};

void main()
{
	outputColor = color;
	gl_Position = pos;		//vec4(in_position, 1.0f);
	oProj = gWorld;
}