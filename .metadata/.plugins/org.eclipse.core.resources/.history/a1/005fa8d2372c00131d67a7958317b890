#version 400
layout(location = 0) in vec3 position;
//layout(location = 1) in vec3 color;

out vec3 outputColor;

layout (std140) uniform ColorBlock
{
	vec3 pos;
	vec3 color;
};

void main()
{
	outputColor = color;
	gl_Position = vec4(position, 1.0f);
}