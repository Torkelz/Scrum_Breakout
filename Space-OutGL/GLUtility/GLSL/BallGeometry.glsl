#version 400

layout (points, invocations = 2) in;
layout (triangle_strip, max_vertices = 4) out;

layout uniform uniMat
{
	mat4 uniTrans;
	mat4 projView;
};

in vertexData
{
	vec3 eyePos;
	vec2 size;
} vertexIn;



void main()
{
	vec3 t_pos;
	t_pos = gl_in[0].gl_Position.xyz;
	
	/*
	t_pos.x = gl_in[0].gl_Position.x;
	t_pos.y = gl_in[0].gl_Position.y;
	t_pos.z = gl_in[0].gl_Position.z;
	*/
	
	vec3 look = normalize(vertexIn.eyePos.xyz - t_pos);
	vec3 right = normalize(cross(vec3(0,1,0), look));
	vec3 up = vec3( 0.0f, 1.0f, 0.0f ); // to make the particles turn around the y-axis
	
	mat4x4 world;
	world[0] = vec4(right, 0.0f);
	world[1] = vec4(up, 0.0f);
	world[2] = vec4(look, 0.0f);
	world[3] = vec4(t_pos, 1.0f);

	mat4x4 PVW = world * uniTrans; //Might need to switch order on mul
	PVW = PVW * projView;

    float halfsize = vertexIn.size * 0.5;
	
	

    gl_TexCoord[0] = gl_TexCoordIn[0][0];
    //gl_FrontColor = gl_FrontColorIn[0];

    //eye_position = gl_PositionIn[0];

    // Vertex 1
    gl_TexCoord[0].st = vec2(-1.0,-1.0);
    gl_Position = gl_In[0].gl_Position;
    gl_Position.xy += vec2(-halfsize, -halfsize);
    gl_Position = PVW * gl_Position;
    EmitVertex();

    // Vertex 2
    gl_TexCoord[0].st = vec2(-1.0,1.0);
    gl_Position = gl_In[0].gl_Position;
    gl_Position.xy += vec2(-halfsize, halfsize);
    gl_Position = PVW * gl_Position;
    EmitVertex();

    // Vertex 3
    gl_TexCoord[0].st = vec2(1.0,-1.0);
    gl_Position = gl_In[0].gl_Position;
    gl_Position.xy += vec2(halfsize, -halfsize);
    gl_Position = PVW * gl_Position;
    EmitVertex();

    // Vertex 4
    gl_TexCoord[0].st = vec2(1.0,1.0);
    gl_Position = gl_In[0].gl_Position;
    gl_Position.xy += vec2(halfsize, halfsize);
    gl_Position = PVW * gl_Position;
    EmitVertex();

    EndPrimitive();
}