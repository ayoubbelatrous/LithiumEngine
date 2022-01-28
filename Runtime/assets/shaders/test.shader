#shader vertex
#version 330 core

layout(location = 0) in vec3 a_Position;

uniform mat4 u_projection;

void main()
{
	
	gl_Position = u_projection * vec4(a_Position, 1.0);
}

#shader fragment
#version 330 core

layout(location = 0) out vec4 color;


void main()
{

	color = vec4(1.0f);
}