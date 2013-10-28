#version 400
layout(location = 0) in vec3 in_position;
layout(location = 1) in vec2 in_texCoord;

out vec2 texCoord;

uniform uniPad
{
	mat4x4 PVW;
};

void main()
{
	gl_Position = PVW *vec4(in_position,1.0);
	texCoord = in_texCoord;
};