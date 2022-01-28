#shader vertex
#version 330 core

layout(location = 0) in vec3 a_Position;

uniform mat4 projection;

void main()
{

	gl_Position = projection * vec4(a_Position, 1.0);
}

#shader fragment
#version 330 core

layout(location = 0) out vec4 color;

layout(location = 1) out int color2;




void main()
{



	color = vec4(1.0,0.0,0.0,1.0);
	color2 = -1;
}