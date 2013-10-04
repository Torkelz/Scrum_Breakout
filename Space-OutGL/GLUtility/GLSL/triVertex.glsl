#version 400
layout(location = 0) in vec3 in_position;
layout(location = 1) in vec3 in_color;

out vec3 outputColor;

uniform ColorBlock
{
	vec4 pos;
	vec4 color;
};

void main()
{
	outputColor = color.xyz;
	gl_Position = pos;//vec4(in_position, 1.0f);
}