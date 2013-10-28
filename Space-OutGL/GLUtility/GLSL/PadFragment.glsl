#version 400
in vec2 texCoord;
out vec4 fragColor;

uniform sampler2D texPad;

void main()
{
	fragColor = texture2D(texPad,texCoord);
}