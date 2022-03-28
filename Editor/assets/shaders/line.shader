#shader vertex
#version 450 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec4 a_Color;

out vec4 Color;
uniform mat4 u_projection;

void main()
{
	Color = a_Color;
	gl_Position = u_projection * vec4(a_Position, 1.0);
}

#shader fragment
#version 450 core

layout(location = 0) out vec4 color;
layout(location = 1) out int color2;

in vec4 Color;


void main()
{
	color = Color;
}