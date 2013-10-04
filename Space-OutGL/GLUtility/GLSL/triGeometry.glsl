#version 400
layout (points) in;
layout (triangle_strip, max_vertices = 3) out;

in vec3 outputColor[];
out vec3 colorGem;

void main()
{
		gl_Position = gl_in[0].gl_Position;
		colorGem = outputColor[0];
		EmitVertex();
		
		gl_Position = vec4(gl_in[0].gl_Position.x + 2, gl_in[0].gl_Position.y, gl_in[0].gl_Position.z, 1.0f);
		colorGem = outputColor[0];
		EmitVertex();
		
		gl_Position = vec4(gl_in[0].gl_Position.x + 1, gl_in[0].gl_Position.y + 2, gl_in[0].gl_Position.z, 1.0f);
		colorGem = outputColor[0];
		EmitVertex();
	EndPrimitive();
}