#version 400
in vec4 colorGem;

out vec4 fragColor;

uniform sampler2D texUnit;

void main()
{
	fragColor = texture(texUnit, colorGem.xy);
}