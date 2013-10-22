#version 400
 
in vec3 inPosition;
in vec3 inEyePos;
in vec3 inSize;

out vertexData
{
	vec3 eyePos;
	vec2 size;
} vertexOut;

void main()
{
    gl_Position = vec4(inPosition, 1.0f);
	vertexOut.eyePos = inEyePos;
	vertexOut.size = inSize;
}