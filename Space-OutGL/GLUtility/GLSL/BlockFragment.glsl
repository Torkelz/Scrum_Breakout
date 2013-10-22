#version 400
in vec4 colorGem;
in vec2 texCoord;
out vec4 fragColor;

uniform sampler2D texBlock;

void main()
{
	fragColor = texture(texBlock,texCoord);
	//fragColor = vec4(texCoord,0,0);
}