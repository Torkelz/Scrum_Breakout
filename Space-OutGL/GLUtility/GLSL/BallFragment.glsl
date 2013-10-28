#version 400

uniform sampler2D texBall;

in vec2 texCoord;
out vec4 fragColor;

void main()
{
	fragColor = texture2D(texBall, texCoord);
}

//http://www.lighthouse3d.com/tutorials/glsl-core-tutorial/glsl-core-tutorial-texturing-with-images/