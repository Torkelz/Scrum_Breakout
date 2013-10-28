#version 400

layout (points) in;
layout (triangle_strip, max_vertices = 4) out;

out vec2 texCoord;

uniform uniBall
{
	mat4 uniTrans;
	mat4 projView;
	vec4 eyePos;
	vec4 size;
};

void main()
{
	vec3 Pos = gl_in[0].gl_Position.xyz;
	
	vec3 toCamera = normalize(eyePos.xyz - Pos.xyz);
	vec3 up = vec3(0.0f, 1.0f, 0.0f);
	vec3 right = cross(toCamera, up);
	
	mat4x4 world;
	world[0] = vec4(right, 0.0f);
	world[1] = vec4(up, 0.0f);
	world[2] = vec4(toCamera, 0.0f);
	world[3] = vec4(Pos.xyz, 1.0f);
	float halfWidth = size.x * 0.5f;
	float halfHeight = size.y * 0.5f;
	
	vec4 v[4];
	v[0] = vec4(-halfWidth, -halfHeight, 0.0f, 1.0f);
	v[1] = vec4(+halfWidth, -halfHeight, 0.0f, 1.0f);
	v[2] = vec4(-halfWidth, +halfHeight, 0.0f, 1.0f);
	v[3] = vec4(+halfWidth, +halfHeight, 0.0f, 1.0f);
	
	
	Pos.y += size.y;
    Pos -= right*size.x;
    gl_Position = projView * uniTrans * world * v[0];
    texCoord = vec2(0.0f, 0.0f);
    EmitVertex();

    Pos.y += size.y;
    gl_Position = projView * uniTrans * world * v[1];
    texCoord = vec2(0.0f, 1.0f);
    EmitVertex();

    Pos.y -= size.y;
    Pos += right*size.x;
    gl_Position = projView * uniTrans * world * v[2];
    texCoord = vec2(1.0f, 0.0f);
    EmitVertex();

    Pos.y += size.y;
    gl_Position = projView * uniTrans * world * v[3];
    texCoord = vec2(1.0f, 1.0f);
    EmitVertex();

    EndPrimitive();
}